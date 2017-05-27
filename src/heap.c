// TODO finalizar documentação

#include <stdlib.h>
#include <string.h> //memcpy
#include <heap.h>

#define HEAP_STARTING_SIZE 512

/** @struct HEAP
 *  @brief Estrutura que implementa a uma heap "inplace" em um vetor.
 */
struct HEAP {
  void *data; /*< Ponteiro para a área de memória que contém os dados. */
  size_t data_size; /*< Tamanho de cada elemento. */
  size_t n_memb; /*< Quantidade de elementos armazenados. */
  size_t max_memb; /*< Quantidade máxima de elementos armazenáveis. */
  int (*compare)(void *a, void *b); /*< Ponteiro para uma função capaz de comparar elementos armazenados. */
};

#define parent(i) ((size_t)(((i)-1) >> 1)) // parent(i) = i - 1 / 2
#define left_child(i) ((size_t)(((i) << 1)+1)) // left(i) = i * 2 + 1
#define right_child(i) ((size_t)(((i) << 1)+2)) // right(i) = i * 2 + 2

// deve ser chamado somente em um escopo em que "heap" seja uma variavel do tipo heap_p inicializada
#define is_valid(i) ((i) < heap->n_memb)

#define HEAP_AT(heap, i) ((heap->data + heap->data_size * (i)))

// deve ser chamado somente em um escopo em que "heap" seja uma variavel do tipo heap_p inicializada
#define heap_at(i) (HEAP_AT(heap, i))

#define HEAP_ASSIGN(dst, src, size) ((memcpy(dst, src, size)))

// deve ser chamado somente em um escopo em que "heap" seja uma variavel do tipo heap_p inicializada
#define heap_assign(dst, src) (HEAP_ASSIGN(dst, src, heap->data_size))

/** @brief Trata o espaço alocado no vetor usado para representar a heap.
 *
 *  Dobra o espaço do vetor alocado para a heap caso este esteja cheio e reduz seu tamanho à metade
 *  caso este esteja com ocupação inferior a 25%. Caso nenhuma das condições acima seja satisfeita
 *  a função não modifica o vetor.
 *
 *  @param heap Ponteiro para a heap cujo espaço deseja-se verificar.
 */
void heap_handle_space(heap_p heap);

/** @brief Troca os índices entre dois elementos da heap.
 *  @param heap Heap onde as operações serão executadas.
 *  @param a Elemento cuja posição será trocada com b.
 *  @param b Elemento cuja posição será trocada com a.
 */
void heap_swap(heap_p heap, size_t a, size_t b);

/* TODO adicionar definições sobrecarregadas de heap_init para permitir inicializar a heap com vetor
 * pré alocado ou com tamanho inicial diferente.
 */
heap_p heap_init(size_t data_size, int (*compare)(void *a, void *b)) {
  heap_p new = malloc(sizeof((*new)));
  new->data = malloc(data_size * HEAP_STARTING_SIZE);
  new->data_size = data_size;
  new->n_memb = 0;
  new->max_memb = HEAP_STARTING_SIZE;
  new->compare = compare;

  return new;
}

heap_p heap_destroy(heap_p heap) {
  if(heap != NULL) {
    if(heap->data != NULL)
      free(heap->data);
  }
  free(heap);
  return NULL;
}

void heap_swap(heap_p heap, size_t a, size_t b) {
  void *tmp = malloc(sizeof(heap->data_size));

  // tmp = V[a];
  heap_assign(tmp, heap_at(a));
  // V[a] = V[b];
  heap_assign(heap_at(a), heap_at(b));
  // V[b] = tmp;
  heap_assign(heap_at(b), tmp);

  free(tmp);
}

void heap_handle_space(heap_p heap) {
  // se espaço insuficiente no vetor, então dobrar seu tamanho
  if(heap->n_memb == heap->max_memb) {
    heap->max_memb = heap->max_memb << 1;
    heap->data = realloc(heap->data, heap->max_memb);
  }
  // se ocupação menor que 25% e tamanho alocado maior que inicial, então reduzir tamanho alocado à
  // metade
  else if(heap->n_memb < heap->max_memb >> 2 && heap->max_memb > HEAP_STARTING_SIZE) {
    heap->max_memb = heap->max_memb >> 1;
    heap->data = realloc(heap->data, heap->max_memb);
  }
}

void heap_heapify_up(heap_p heap, size_t i) {
  size_t p;
  while(i != 0 && heap->compare(heap_at(i), heap_at(p = parent(i))) > 0) {
    // enquanto elemento é maior que seu elemento pai, trocar
    heap_swap(heap, i, p);
    i = p;
  }
}

void heap_heapify_down(heap_p heap, size_t i) {
  size_t l, r, largest;
  do {
    l = left_child(i), r = right_child(i), largest = i;
    if(is_valid(r)) { // se existe filho à direita, então existe filho à esquerda
      // se (esquerda > maior), então (maior = esquerda)
      if(heap->compare(heap_at(l), heap_at(largest)) > 0)
        largest = l;
      // se (direita > maior), então (maior = direita)
      if(heap->compare(heap_at(r), heap_at(largest)) > 0)
        largest = r;
    } else if (is_valid(l)) { // senao, se existe filho à esquerda
      // se (esquerda > maior), então (maior = esquerda)
      if(heap->compare(heap_at(l), heap_at(largest)) > 0)
        largest = l;
    } else break; // se não tem filhos, então está na posição correta

    // se i não é o maior, trocar V[i] com V[maior]
    if(largest != i) {
      heap_swap(heap, i, largest);
      i = largest;
    } else break;
  } while(1);
}

void heap_insert(heap_p heap, void *data) {
  // verificar se há espaço suficiente
  heap_handle_space(heap);

  // V[n_memb] = *data
  heap_assign(heap_at(heap->n_memb), data);
  // incrementar n_memb
  heap->n_memb++;

  // verificar propriedade da heap a partir do elemento inserido
  heap_heapify_up(heap, heap->n_memb - 1);
}

void heap_pop(heap_p heap, void *out) {
  // out = V[0]
  heap_assign(out, heap_at(0));
  // decrementar n_memb;
  heap->n_memb--;

  if (heap->n_memb != 0) { // se n_memb == 0, 'memcpy()' fará uma cópia de endereços sobrepostos
    // V[0] = V[n_memb - 1]
    heap_assign(heap_at(0), heap_at(heap->n_memb));

    // verificar se ocupação está muito baixa.
    heap_handle_space(heap);

    // verificar heap a partir da raiz, que foi substituída pelo último elemento
    heap_heapify_down(heap, 0);
  }
}

void *heap_array_pointer(heap_p heap) {
  return heap->data;
}

size_t heap_size(heap_p heap) {
  return heap->n_memb;
}
