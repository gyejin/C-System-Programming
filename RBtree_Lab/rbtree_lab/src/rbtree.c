#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void);
void free_recursive(node_t *node, node_t *nil);
void delete_rbtree(rbtree *t);
void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *y);
node_t *rbtree_insert(rbtree *t, const key_t key);
void rb_insert_fixup(rbtree *t, node_t *z);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));    //트리 하나 동적할당
  if (p == NULL) { return NULL; }    //메모리 할당 됐는지 확인

  //nil노드 하나 만들어서 재활용 (1개 만듦), 초깃값 초기화 귀찮아서 calloc씀(color, key, parent, left, right)
  node_t *nil_leaf = (node_t *)calloc(1, sizeof(node_t));
  if (nil_leaf == NULL) { return NULL; }    //Nil노드 할당 됐는지 확인
  
  nil_leaf->color = RBTREE_BLACK; //모든 리프(nil 노드)는 검은색

  //초기값
  p->root = nil_leaf;   //루트는 nil을 가리키는게 초기
  p->nil = nil_leaf;    //nil도 nil가리킴
  
  return p;
}

void free_recursive(node_t *node, node_t *nil) {
  // 1. (Base Case) 현재 노드가 nil(센티넬) 노드이면, 더 이상 진행할 필요가 없으므로 함수를 종료합니다.
  if (node == nil) { return; }

  // 2. (재귀) 왼쪽 서브트리를 먼저 해제합니다.
  free_recursive(node->left, nil);

  // 3. (재귀) 오른쪽 서브트리를 해제합니다.
  free_recursive(node->right, nil);
  
  // 4. (후위 순회) 왼쪽과 오른쪽 자식들이 모두 해제된 후, 마지막으로 현재 노드 자체를 해제합니다.
  free(node);
}

void delete_rbtree(rbtree *t) {
  // 1. 트리가 NULL인지 먼저 확인하는 것이 안전합니다.
  if (t == NULL) { return; }

  // 2. root 노드부터 시작해서 모든 노드들의 메모리를 재귀적으로 해제합니다.
  free_recursive(t->root, t->nil);

  // 3. 모든 일반 노드들이 해제된 후, 처음에 딱 한 번 만들었던 센티넬(nil) 노드를 해제합니다.
  free(t->nil);

  // 4. 마지막으로, 트리 구조체 자체의 메모리를 해제합니다.
  free(t);
}

void left_rotate(rbtree *t, node_t *x) {
  // y를 x의 오른쪽 자식으로 지정합니다. 이 y 노드가 x의 자리로 올라올 것입니다.
  node_t *y = x->right;

  // y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 연결합니다.
  // 이로써 y가 떠나면서 갈 곳을 잃은 y의 왼쪽 자식('β')이 x에게 입양됩니다.
  x->right = y->left;

  // 만약 y의 왼쪽 서브트리가 실제로 존재했다면(nil이 아니었다면),
  // 이제 그 서브트리의 부모는 x가 되었음을 알려주어야 합니다.
  if (y->left != t->nil){
    y->left->parent = x;
  }

  // y의 부모를 x의 부모로 설정하여, y가 x의 자리를 물려받을 준비를 합니다.
  y->parent = x->parent;

  // x의 원래 위치에 따라 y의 새로운 위치를 확정합니다.
  // x의 부모 노드 입장에서, 자신의 자식이 x에서 y로 바뀌었음을 인지해야 합니다.
  
  // Case 1: x가 트리의 루트였다면, 이제 y가 새로운 루트가 됩니다.
  if (x->parent == t->nil) {
    t->root = y;
  }
  // Case 2: x가 원래 부모의 왼쪽 자식이었다면, 이제 y가 그 자리를 차지합니다.
  else if (x == x->parent->left) {
    x->parent->left = y;
  }
  // Case 3: x가 원래 부모의 오른쪽 자식이었다면, 이제 y가 그 자리를 차지합니다.
  else {
    x->parent->right = y;
  }

  // 마지막으로, x와 y의 관계를 최종적으로 정리합니다.
  // x는 y의 왼쪽 자식이 되고, x의 부모는 y가 됩니다.
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y) {
  node_t *x = y->left;

  y->left = x->right;

  if (x->right != t->nil){
    x->right->parent = y;
  }

  x->parent = y->parent;
  
  // Case 1: y가 트리의 루트였다면, 이제 x가 새로운 루트가 됩니다.
  if (y->parent == t->nil) {
    t->root = x;
  }
  // Case 2: y가 원래 부모의 오른쪽 자식이었다면, 이제 x가 그 자리를 차지합니다.
  else if (y == y->parent->right) {
    y->parent->right = x;
  }
  // Case 3: y가 원래 부모의 왼쪽 자식이었다면, 이제 x가 그 자리를 차지합니다.
  else {
    y->parent->left = x;
  }

  // 마지막으로, x와 y의 관계를 최종적으로 정리합니다.
  // y는 x의 오른쪽 자식이 되고, y의 부모는 x가 됩니다.
  x->right = y;
  y->parent = x;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // [ Part 1: 일반적인 BST의 삽입 과정 ]

  // 1. 삽입될 새로운 노드 'z'를 위한 메모리를 할당합니다.
  node_t *z = (node_t *)malloc(sizeof(node_t));
  
  // 2. 새 노드 'z'의 기본 값을 설정합니다.
  //    - key는 입력받은 key 값
  //    - color는 항상 RED (RB 트리 규칙!)
  //    - left, right 포인터는 nil(센티넬) 노드를 가리키도록 설정
  z->key = key;
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;
  z->parent = t->nil; // 부모는 아직 미정

  // 3. 삽입될 위치를 찾기 위한 포인터들을 준비합니다.
  //    y는 z의 미래 부모가 될 노드, x는 트리를 탐색할 현재 위치 포인터입니다.
  node_t *y = t->nil;
  node_t *x = t->root;

  // 4. 트리의 바닥(nil)에 도달할 때까지 반복하며 삽입 위치를 찾습니다.
  while (x != t->nil) {
    // y는 x를 한 칸씩 뒤따라갑니다.
    y = x; 
    
    // key 값을 비교하여 왼쪽으로 갈지, 오른쪽으로 갈지 결정합니다.
    // (multiset이므로 같을 때는 오른쪽으로 갑니다)
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  // 5. 이제 y는 z가 삽입될 위치의 부모 노드가 되었습니다. z의 부모를 y로 설정합니다.
  z->parent = y;

  // 6. y의 상태에 따라 z를 최종 위치에 연결합니다.
  //    Case 1: y가 nil이라면 (즉, 트리가 원래 비어있었다면), z가 새로운 루트가 됩니다.
  if (y == t->nil) {
    t->root = z;
  }
  //    Case 2: z의 key가 부모(y)의 key보다 작다면, z를 y의 왼쪽 자식으로 연결합니다.
  else if (z->key < y->key) {
    y->left = z;
  }
  //    Case 3: 그 외의 경우, z를 y의 오른쪽 자식으로 연결합니다.
  else {
    y->right = z;
  }

  // [ Part 2: RB 트리 속성 유지 ]
  rb_insert_fixup(t, z);
  
  return z;
}

void rb_insert_fixup(rbtree *t, node_t *z) {
  // z의 부모가 RED인 동안에만 루프를 실행합니다. (더블 레드 문제)
  while (z->parent->color == RBTREE_RED) {
    
    // Case A: z의 부모가 할아버지의 '왼쪽' 자식인 경우
    if (z->parent == z->parent->parent->left) {
      node_t *uncle = z->parent->parent->right; // 삼촌 노드를 찾습니다.

      // Case 1: 삼촌이 RED인 경우
      if (uncle->color == RBTREE_RED) {
        // 부모와 삼촌을 BLACK으로, 할아버지를 RED로 바꿉니다 (Recoloring).
        // 그 다음, z를 할아버지 위치로 옮겨서 루프를 계속 확인합니다.
        uncle->color = RBTREE_BLACK;
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } 
      // Case 2 & 3: 삼촌이 BLACK인 경우
      else {
        // Case 2: z가 부모의 '오른쪽' 자식인 경우 (꺾인 모양)
        if (z == z->parent->right) {
          // z를 부모 위치로 올리고 left_rotate를 실행하여 Case 3 모양으로 만듭니다.
          z = z->parent;
          left_rotate(t, z);
        }
        // Case 3: z가 부모의 '왼쪽' 자식인 경우 (직선 모양)
        // 부모를 BLACK으로, 할아버지를 RED로 바꾼 뒤, 할아버지를 기준으로 right_rotate를 실행합니다.
        // 이 케이스를 거치면 루프가 종료됩니다.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
        break;
      }
    } 
    // Case B: z의 부모가 할아버지의 '오른쪽' 자식인 경우 (Case A와 완벽히 대칭)
    else {
      node_t *uncle = z->parent->parent->left; // 삼촌 노드

      // Case 1: 삼촌이 RED인 경우
      if (uncle->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // Case 2 & 3: 삼촌이 BLACK인 경우
      else {
        // Case 2: z가 부모의 '왼쪽' 자식인 경우
        if (z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }
        // Case 3: z가 부모의 '오른쪽' 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  // 루프가 끝나면, RB 트리의 2번 규칙 "루트는 BLACK이다"를 다시 한번 확인사살해줍니다.
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // 1. 검색을 시작할 현재 노드(p)를 트리의 루트로 설정합니다.
  node_t *p = t->root;

  // 2. p가 nil 노드가 아닐 동안 계속 반복하여 탐색합니다.
  while (p != t->nil) {
    // 찾으려는 key가 현재 노드의 key와 같으면, p를 반환합니다.
    if (p->key == key) {
      return p;
    }
    // 찾으려는 key가 현재 노드의 key보다 작으면, 왼쪽으로 이동합니다.
    else if (p->key > key) {
      p = p->left;
    }
    // 찾으려는 key가 현재 노드의 key보다 크면, 오른쪽으로 이동합니다.
    else {
      p = p->right;
    }
  }

  // while 루프를 빠져나왔다는 것은 값을 찾지 못했다는 의미입니다.
  // NULL을 반환합니다.
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // 1. 트리가 비어있는지 확인합니다. 만약 루트가 nil이면, 트리에 노드가 없다는 뜻이죠.
  //    이 경우엔 그냥 루트(nil)를 반환하면 됩니다.
  if (t->root == t->nil) {
    return t->nil;
  }

  // 2. 탐색을 시작할 현재 노드(p)를 트리의 루트로 설정합니다.
  node_t *p = t->root;

  // 3. 현재 노드의 왼쪽 자식이 nil이 아닐 동안, 계속 왼쪽으로 이동합니다.
  while (p->left != t->nil) {
    p = p->left;
  }

  // 4. while 루프가 끝났다면, 가장 왼쪽에 도달한 것입니다. 현재 노드(p)를 반환합니다.
  return p;
}

node_t *rbtree_max(const rbtree *t) {
  // 1. 트리가 비어있는지 확인합니다.
  if (t->root == t->nil){
    return t->nil;
  }

  // 2. 탐색을 시작할 현재 노드(p)를 트리의 루트로 설정합니다.
  node_t *p = t->root;

  // 3. 현재 노드의 오른쪽 자식이 nil이 아닐 동안, 계속 오른쪽으로 이동합니다.
  while (p->right != t->nil){
    p = p->right;
  }

  // 4. 가장 오른쪽에 도달했다면, 현재 노드(p)를 반환합니다.
  return p;
}


// [재귀 호출용 중위 순회 함수]
// 이 함수는 rbtree_to_array 안에서만 사용되므로 'static'으로 선언합니다.
// node: 현재 방문 중인 노드
// arr: key 값을 저장할 배열
// index: 배열의 현재 채워진 위치
// n: 배열의 최대 크기
int inorder_recursive(const rbtree *t, node_t *node, key_t *arr, int index, const size_t n) {
  // Base Case: 현재 노드가 nil이거나, 배열이 꽉 찼으면 더 이상 진행하지 않고 현재 index를 반환합니다.
  if (node == t->nil || index >= n) {
    return index;
  }

  // 1. (재귀) 왼쪽 서브트리를 먼저 순회합니다.
  //    왼쪽 순회가 끝난 후 업데이트된 index 값을 받아와야 합니다.
  index = inorder_recursive(t, node->left, arr, index, n);

  // 배열이 꽉 찼는지 다시 한번 확인합니다. (왼쪽 순회 중에 꽉 찼을 수도 있어요!)
  if (index >= n) {
    return index;
  }

  // 2. (자기 자신 처리) 현재 노드의 key 값을 배열에 저장하고, index를 1 증가시킵니다.
  arr[index] = node->key;
  index++;

  // 3. (재귀) 오른쪽 서브트리를 순회합니다.
  //    오른쪽 순회가 끝난 후 업데이트된 index 값을 받아와야 합니다.
  index = inorder_recursive(t, node->right, arr, index, n);

  // 최종적으로 업데이트된 index를 반환합니다.
  return index;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // 1. 트리가 비어있거나 배열 크기가 0이면 할 일이 없으므로 0을 반환합니다.
  if (t->root == t->nil || n == 0) {
    return 0;
  }

  // 2. 루트 노드부터 중위 순회를 시작하는 재귀 도우미 함수를 호출합니다.
  //    초기 index는 0으로 시작합니다.
  //    이 함수가 반환하는 값은 배열에 최종적으로 몇 개의 원소가 채워졌는지를 나타냅니다.
  return inorder_recursive(t, t->root, arr, 0, n);
}

// [삭제 도우미 함수: Transplant]
// u 노드의 자리를 v 노드로 교체하는 함수입니다.
void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  // Case 1: u가 루트였다면, v를 새로운 루트로 만듭니다.
  if (u->parent == t->nil) {
    t->root = v;
  }
  // Case 2: u가 부모의 왼쪽 자식이었다면, v를 그 자리에 둡니다.
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  // Case 3: u가 부모의 오른쪽 자식이었다면, v를 그 자리에 둡니다.
  else {
    u->parent->right = v;
  }

  // 마지막으로, v의 부모를 u의 부모로 설정합니다. (v가 nil일 수도 있으니 주의!)
  v->parent = u->parent;
}

// [삭제 도우미 함수: Successor 찾기]
// 특정 노드(p)의 오른쪽 서브트리에서 가장 작은 값을 가진 노드를 찾는 함수가 필요해요.
// rbtree_min 함수와 거의 똑같지만, 시작점을 root가 아닌 특정 노드로 잡는다는 점만 달라요.
// 이 함수는 erase 함수 내에서만 사용할 것이므로 static으로 선언하는 것이 좋습니다.
node_t *find_successor(rbtree *t, node_t *p) {
    // p의 오른쪽 서브트리에서 가장 왼쪽 노드를 찾아 반환하세요.
    node_t *cur = p->right;
    while (cur->left != t->nil){
      cur = cur->left;
    }
    return cur;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z; // y: 실제로 삭제/이동될 노드
  node_t *x;     // x: y의 원래 자리를 대체할 노드
  color_t y_original_color = y->color;

  // Case 1: 삭제할 노드(z)의 왼쪽 자식이 없을 때
  if (z->left == t->nil) {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  // Case 2: 삭제할 노드(z)의 오른쪽 자식이 없을 때
  else if (z->right == t->nil) {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  // Case 3: 삭제할 노드(z)의 자식이 둘 다 있을 때
  else {
    // y를 z의 successor(오른쪽 서브트리에서 가장 작은 노드)로 설정합니다.
    y = find_successor(t, z);
    y_original_color = y->color;
    x = y->right; // x는 successor의 오른쪽 자식 (successor는 왼쪽 자식이 없음)
    
    // y가 z의 직접적인 자식이 아닐 경우, y를 먼저 z의 자리로 옮기기 위한 추가 작업이 필요합니다.
    if (y->parent != z) {
        rb_transplant(t, y, y->right);
        y->right = z->right;
        y->right->parent = y;
    }
    // y가 z의 바로 오른쪽 자식인 경우
    else {
      x->parent = y; // x가 nil일 경우를 대비해 부모를 직접 설정
    }
    // y를 최종적으로 z의 위치로 옮깁니다.
    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  
  // 삭제/이동된 노드(y)의 원래 색이 BLACK이었다면, 규칙이 깨졌을 수 있습니다.
  // fixup 함수를 호출하여 트리를 복구합니다.
  if (y_original_color == RBTREE_BLACK) {
    rb_erase_fixup(t, x); // 다음 미션에서 구현!
  }

  // 삭제된 노드 z의 메모리를 해제합니다.
  free(z);
  return 0;
}

void rb_erase_fixup(rbtree *t, node_t *x) {
  // x가 루트가 아니고, "엑스트라 블랙"을 가지고 있는 동안 루프를 반복합니다.
  while (x != t->root && x->color == RBTREE_BLACK) {
    
    // Case A: x가 부모의 '왼쪽' 자식인 경우
    if (x == x->parent->left) {
      node_t *w = x->parent->right; // w는 x의 형제 노드

      // Case 1: 형제 w가 RED인 경우
      if (w->color == RBTREE_RED) {
        // 색상을 바꾸고 부모를 기준으로 left_rotate를 합니다.
        // 이 과정을 통해 x의 새로운 형제는 BLACK이 되어 Case 2, 3, 4 중 하나로 귀결됩니다.
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      // Case 2: 형제 w가 BLACK이고, w의 두 자식 모두 BLACK인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // 형제 w를 RED로 바꾸고, "엑스트라 블랙"을 부모에게 넘깁니다.
        // x를 부모로 설정하여 루프를 계속 진행합니다.
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // Case 3 & 4: 형제 w가 BLACK이고, 자식 중 RED가 있는 경우
      else {
        // Case 3: w의 오른쪽 자식이 BLACK인 경우 (왼쪽 자식은 RED)
        if (w->right->color == RBTREE_BLACK) {
          // 색상을 바꾸고 w를 기준으로 right_rotate를 합니다.
          // 이 과정을 통해 Case 4 모양으로 만듭니다.
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        
        // Case 4: w의 오른쪽 자식이 RED인 경우
        // 색상을 적절히 바꾸고, 부모를 기준으로 left_rotate를 합니다.
        // 이 과정을 거치면 "엑스트라 블랙" 문제가 해결되므로, x를 루트로 만들어 루프를 종료합니다.
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    } 
    // Case B: x가 부모의 '오른쪽' 자식인 경우 (Case A와 완벽히 대칭)
    else {
      node_t *w = x->parent->left; // w는 x의 형제 노드

      // Case 1: 형제 w가 RED인 경우
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      // Case 2: 형제 w가 BLACK이고, w의 두 자식 모두 BLACK인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // Case 3 & 4: 형제 w가 BLACK이고, 자식 중 RED가 있는 경우
      else {
        // Case 3: w의 왼쪽 자식이 BLACK인 경우 (오른쪽 자식은 RED)
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // Case 4: w의 왼쪽 자식이 RED인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  // 루프가 끝난 후, x가 물려받았던 "엑스트라 블랙"을 진짜 BLACK으로 만들어줍니다.
  x->color = RBTREE_BLACK;
}