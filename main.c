#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artista {
  char nome[100];
  char genero[100];
  char localizacao[100];
  char albuns[1000];
  struct Artista *proximo;
} Artista;

Artista *criarArtista(const char *nome, const char *genero, const char *localizacao, const char *albuns) {
  Artista *novoArtista = (Artista *)malloc(sizeof(Artista));
  if (novoArtista == NULL) {
    perror("Erro ao alocar memória para o artista");
    exit(1);
  }
  strcpy(novoArtista->nome, nome);
  strcpy(novoArtista->genero, genero);
  strcpy(novoArtista->localizacao, localizacao);
  strcpy(novoArtista->albuns, albuns);
  novoArtista->proximo = NULL;
  return novoArtista;
}

Artista *inserirArtistaOrdenado(Artista *cabeca, Artista *novoArtista) {
  if (cabeca == NULL || strcmp(novoArtista->nome, cabeca->nome) < 0) {
    novoArtista->proximo = cabeca;
    return novoArtista;
  }

  Artista *atual = cabeca;
  while (atual->proximo != NULL && strcmp(novoArtista->nome, atual->proximo->nome) >= 0) {
    atual = atual->proximo;
  }

  novoArtista->proximo = atual->proximo;
  atual->proximo = novoArtista;
  return cabeca;
}

Artista *removerArtista(Artista *cabeca, const char *nome) {
  Artista *anterior = NULL;
  Artista *atual = cabeca;

  while (atual != NULL && strcmp(nome, atual->nome) != 0) {
    anterior = atual;
    atual = atual->proximo;
  }

  if (atual == NULL) {
    printf("Artista não encontrado: %s\n", nome);
    return cabeca;
  }

  if (anterior == NULL) {
    cabeca = atual->proximo;
  } else {
    anterior->proximo = atual->proximo;
  }

  free(atual);
  return cabeca;
}

Artista *buscarArtistaBinario(Artista *cabeca, const char *nome) {
  Artista *atual = cabeca;
  while (atual != NULL) {
    int cmp = strcmp(nome, atual->nome);
    if (cmp == 0) {
      return atual;
    } else if (cmp < 0) {
      return NULL;
    }
    atual = atual->proximo;
  }
  return NULL;
}

Artista *buscarAlbum(Artista *cabeca, const char *album) {
  Artista *atual = cabeca;
  while (atual != NULL) {
    char *albumPtr = strstr(atual->albuns, album);
    if (albumPtr != NULL) {
      return atual;
    }
    atual = atual->proximo;
  }
  return NULL;
}

void imprimirArtista(const Artista *artista) {
  printf("Nome: %s\n", artista->nome);
  printf("Gênero: %s\n", artista->genero);
  printf("Localização: %s\n", artista->localizacao);
  printf("Álbuns: %s\n", artista->albuns);
  printf("===========\n");
}

void liberarListaArtistas(Artista *cabeca) {
  while (cabeca != NULL) {
    Artista *atual = cabeca;
    cabeca = cabeca->proximo;
    free(atual);
  }
}

int main() {
  FILE *arquivo = fopen("artistas.txt", "r");
  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo artistas.txt");
    return 1;
  }

  Artista *listaArtistas = NULL;
  char linha[1000];
  char nome[100], genero[100], localizacao[100], albuns[1000];
  while (fgets(linha, sizeof(linha), arquivo)) {
    if (strcmp(linha, "===========\n") == 0) {
      Artista *novoArtista = criarArtista(nome, genero, localizacao, albuns);
      listaArtistas = inserirArtistaOrdenado(listaArtistas, novoArtista);
    } else {
      if (strcmp(linha, "\n") != 0) {
        if (linha[strlen(linha) - 1] == '\n') {
          linha[strlen(linha) - 1] = '\0';
        }
        if (strcmp(nome, "") == 0) {
          strcpy(nome, linha);
        } else if (strcmp(genero, "") == 0) {
          strcpy(genero, linha);
        } else if (strcmp(localizacao, "") == 0) {
          strcpy(localizacao, linha);
        } else {
          strcat(albuns, linha);
          strcat(albuns, "\n");
        }
      }
    }
  }

  fclose(arquivo);

  int escolha;
  char nomeBusca[100];
  char nomeAlbumBusca[1000];
  while (1) {
    printf("\nMenu:\n");
    printf("1. Inserir novo artista\n");
    printf("2. Remover artista\n");
    printf("3. Editar artista\n");
    printf("4. Buscar artista\n");
    printf("5. Buscar álbum\n");
    printf("6. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);
    getchar();

    switch (escolha) {
    case 1:
      printf("Digite o nome do novo artista: ");
      fgets(nome, sizeof(nome), stdin);
      nome[strlen(nome) - 1] = '\0';
      printf("Digite o gênero musical: ");
      fgets(genero, sizeof(genero), stdin);
      genero[strlen(genero) - 1] = '\0';
      printf("Digite a localização: ");
      fgets(localizacao, sizeof(localizacao), stdin);
      localizacao[strlen(localizacao) - 1] = '\0';
      printf("Digite os álbuns (um por linha, digite 'FIM' para encerrar):\n");
      strcpy(albuns, "");
      while (1) {
        fgets(linha, sizeof(linha), stdin);
        if (strcmp(linha, "FIM\n") == 0) {
          break;
        }
        strcat(albuns, linha);
      }
      Artista *novoArtista = criarArtista(nome, genero, localizacao, albuns);
      listaArtistas = inserirArtistaOrdenado(listaArtistas, novoArtista);

      arquivo = fopen("artistas.txt", "a");
      if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo artistas.txt");
        return 1;
      }
      fprintf(arquivo, "%s\n%s\n%s\n%s===========\n", nome, genero, localizacao,
              albuns);
      fclose(arquivo);

      printf("Artista inserido com sucesso!\n");
      break;

    case 2:
      printf("Digite o nome do artista a ser removido: ");
      fgets(nomeBusca, sizeof(nomeBusca), stdin);
      nomeBusca[strlen(nomeBusca) - 1] = '\0';
      listaArtistas = removerArtista(listaArtistas, nomeBusca);

      arquivo = fopen("artistas.txt", "w");
      if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo artistas.txt");
        return 1;
      }
      Artista *atual = listaArtistas;
      while (atual != NULL) {
        fprintf(arquivo, "%s\n%s\n%s\n%s===========\n", atual->nome,
                atual->genero, atual->localizacao, atual->albuns);
        atual = atual->proximo;
      }
      fclose(arquivo);

      printf("Artista removido com sucesso!\n");
      break;

    case 3:
      printf("Digite o nome do artista a ser editado: ");
      fgets(nomeBusca, sizeof(nomeBusca), stdin);
      nomeBusca[strlen(nomeBusca) - 1] = '\0';
      Artista *editarArtista = buscarArtistaBinario(listaArtistas, nomeBusca);
      if (editarArtista != NULL) {
        printf("Digite o novo nome do artista: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strlen(nome) - 1] = '\0';
        printf("Digite o novo gênero musical: ");
        fgets(genero, sizeof(genero), stdin);
        genero[strlen(genero) - 1] = '\0';
        printf("Digite a nova localização: ");
        fgets(localizacao, sizeof(localizacao), stdin);
        localizacao[strlen(localizacao) - 1] = '\0';
        printf("Digite os novos álbuns (um por linha, digite 'FIM' para "
               "encerrar):\n");
        strcpy(albuns, "");
        while (1) {
          fgets(linha, sizeof(linha), stdin);
          if (strcmp(linha, "FIM\n") == 0) {
            break;
          }
          strcat(albuns, linha);
        }
        strcpy(editarArtista->nome, nome);
        strcpy(editarArtista->genero, genero);
        strcpy(editarArtista->localizacao, localizacao);
        strcpy(editarArtista->albuns, albuns);

        arquivo = fopen("artistas.txt", "w");
        if (arquivo == NULL) {
          perror("Erro ao abrir o arquivo artistas.txt");
          return 1;
        }
        Artista *atual = listaArtistas;
        while (atual != NULL) {
          fprintf(arquivo, "%s\n%s\n%s\n%s===========\n", atual->nome,
                  atual->genero, atual->localizacao, atual->albuns);
          atual = atual->proximo;
        }
        fclose(arquivo);

        printf("Artista editado com sucesso!\n");
      } else {
        printf("Artista não encontrado: %s\n", nomeBusca);
      }
      break;

    case 4:
      printf("Digite o nome do artista a ser buscado: ");
      fgets(nomeBusca, sizeof(nomeBusca), stdin);
      nomeBusca[strlen(nomeBusca) - 1] = '\0';
      Artista *artistaEncontrado =
          buscarArtistaBinario(listaArtistas, nomeBusca);
      if (artistaEncontrado != NULL) {
        printf("Artista encontrado:\n");
        imprimirArtista(artistaEncontrado);
      } else {
        printf("Artista não encontrado: %s\n", nomeBusca);
      }
      break;

    case 5:
      printf("Digite o nome do álbum a ser buscado: ");
      fgets(nomeAlbumBusca, sizeof(nomeAlbumBusca), stdin);
      nomeAlbumBusca[strlen(nomeAlbumBusca) - 1] = '\0';
      Artista *artistaAlbum = buscarAlbum(listaArtistas, nomeAlbumBusca);
      if (artistaAlbum != NULL) {
        printf("Álbum encontrado no artista:\n");
        imprimirArtista(artistaAlbum);
      } else {
        printf("Álbum não encontrado: %s\n", nomeAlbumBusca);
      }
      break;

    case 6:
      liberarListaArtistas(listaArtistas);
      return 0;

    default:
      printf("Opção inválida\n");
    }
  }

  return 0;
}
