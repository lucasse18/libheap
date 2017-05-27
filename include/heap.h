// TODO finalizar documentação

#ifndef _UTIL_HEAP_H
#define _UTIL_HEAP_H


#include <stdlib.h> //size_t

/** @typedef heap_p Declaração do tipo que representa um ponteiro para uma heap.
 */
typedef struct HEAP * heap_p;

/** @brief Aloca memória para as estruturas de controle da heap e as inicializa.
 *  @param data_size Tamanho de cada unidade de dado.
 *  @param compare Função capaz de comparar pares de elementos armazenados.
 */
heap_p heap_init(size_t data_size, int (*compare)(void *a, void *b));
heap_p heap_destroy(heap_p heap);
void heap_insert(heap_p heap, void *data);
void heap_pop(heap_p heap, void *out);

/** @brief Função que retorna um ponteiro para o vetor subjacente que representa a heap.
 *
 *  O tamanho da área de memória iniciada no ponteiro retornado pode ser obtido pela função
 *  'heap_size'. Passar este ponteiro para a função 'free' (stdlib) resulta em comportamento
 *  indefinido nas demais funções que operam sobre o tipo heap_p.
 *
 *  @param heap Heap a ser acessada.
 *  @return Ponteiro para o início da área de dados
 */
void *heap_array_pointer(heap_p heap);

size_t heap_size(heap_p heap);


#endif//_UTIL_HEAP_H
