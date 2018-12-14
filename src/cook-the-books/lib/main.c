#include <stdlib.h>
#include <emscripten.h>

struct Node
{
  int id;
  int categoryId;
  float rawAmount;
  float cookedAmount;
  struct Node *next;
  struct Node *prev;
};

typedef enum
{
  RAW = 1,
  COOKED = 2
} AmountType;

struct Node *transactionsHead = NULL;
struct Node *categoriesHead = NULL;

EMSCRIPTEN_KEEPALIVE
void deleteNode(struct Node **headNode, struct Node *delNode)
{
  if (*headNode == NULL || delNode == NULL)
    return;
  if (delNode->next != NULL)
    delNode->next->prev = delNode->prev;
  if (delNode->prev != NULL)
    delNode->prev->next = delNode->next;
  free(delNode);
}

EMSCRIPTEN_KEEPALIVE
void appendNode(struct Node **headNode, int id, int categoryId, float rawAmount, float cookedAmount)
{
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  struct Node *last = *headNode;

  newNode->id = id;
  newNode->categoryId = categoryId;
  newNode->rawAmount = rawAmount;
  newNode->cookedAmount = cookedAmount;
  newNode->next = NULL;

  if (*headNode == NULL)
  {
    newNode->prev = NULL;
    *headNode = newNode;
    return;
  }

  while (last->next != NULL)
  {
    last = last->next;
  }
  last->next = newNode;
  newNode->prev = last;
}

EMSCRIPTEN_KEEPALIVE
struct Node *findNodeById(int id, struct Node *withinNode)
{
  struct Node *node = withinNode;
  while (node != NULL)
  {
    if (node->id == id)
      return node;
    node = node->next;
  }
  return NULL;
}

EMSCRIPTEN_KEEPALIVE
void addTransaction(int id, int categoryId, float rawAmount, float cookedAmount)
{
  appendNode(&transactionsHead, id, categoryId, rawAmount, cookedAmount);
}

EMSCRIPTEN_KEEPALIVE
void editTransaction(int id, int categoryId, float rawAmount, float cookedAmount)
{
  struct Node *foundNode = findNodeById(id, transactionsHead);
  if (foundNode != NULL)
  {
    foundNode->categoryId = categoryId;
    foundNode->rawAmount = rawAmount;
    foundNode->cookedAmount = cookedAmount;
  }
}

EMSCRIPTEN_KEEPALIVE
void removeTransaction(int id)
{
  struct Node *foundNode = findNodeById(id, transactionsHead);
  if (foundNode != NULL)
    deleteNode(&transactionsHead, foundNode);
}

EMSCRIPTEN_KEEPALIVE
void calculateGrandTotals(float *totalRaw, float *totalCooked)
{
  struct Node *node = transactionsHead;
  while (node != NULL)
  {
    *totalRaw += node->rawAmount;
    *totalCooked += node->cookedAmount;
    node = node->next;
  }
}

EMSCRIPTEN_KEEPALIVE
float getGrandTotalForType(AmountType type)
{
  float totalRaw = 0;
  float totalCooked = 0;
  calculateGrandTotals(&totalRaw, &totalCooked);
  if (type == RAW)
    return totalRaw;
  if (type == COOKED)
    return totalCooked;
  return 0;
}

EMSCRIPTEN_KEEPALIVE
float getFinalBalanceForType(AmountType type, float initialBalance)
{
  float totalForType = getGrandTotalForType(type);
  return initialBalance + totalForType;
}

EMSCRIPTEN_KEEPALIVE
void upsertCategoryNode(int categoryId, float transactionRaw, float transactionCooked)
{
  struct Node *foundNode = findNodeById(categoryId, categoriesHead);
  if (foundNode != NULL)
  {
    foundNode->rawAmount += transactionRaw;
    foundNode->cookedAmount += transactionCooked;
  }
  else
  {
    appendNode(&categoriesHead, categoryId, categoryId, transactionRaw, transactionCooked);
  }
}

EMSCRIPTEN_KEEPALIVE
void buildValuesByCategoryList()
{
  struct Node *node = transactionsHead;
  while (node != NULL)
  {
    upsertCategoryNode(node->categoryId, node->rawAmount, node->cookedAmount);
    node = node->next;
  }
}

EMSCRIPTEN_KEEPALIVE
void recalculateForCategories()
{
  categoriesHead = NULL;
  buildValuesByCategoryList();
}

EMSCRIPTEN_KEEPALIVE
float getCategoryTotal(AmountType type, int categoryId)
{
  if (categoriesHead == NULL)
    buildValuesByCategoryList();
  struct Node *categoryNode = findNodeById(categoryId, categoriesHead);
  if (categoryNode == NULL)
    return 0;
  if (type == RAW)
    return categoryNode->rawAmount;
  if (type == COOKED)
    return categoryNode->cookedAmount;
  return 0;
}
