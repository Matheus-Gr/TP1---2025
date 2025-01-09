#include <time.h>  // Para clock_t
#include "tipos.h"

void inicializarEstatisticas(Estatisticas *estatisticas) {
    estatisticas->transferencias = 0;
    estatisticas->comparacoes = 0;
    estatisticas->inicio = clock();
    estatisticas->tempoExecucao = 0.0;
}
