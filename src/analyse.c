#include "../include/file.h"

char **analyse_arg(const char arg[]) {
  // On saute les espaces en d�but
  const char* parg = arg;
  while (*parg == ' ') {
    ++parg;
  }
  size_t len = strlen(parg) + 1;

  /**
   * On commence par copier arg dans une cha�ne sur laquelle pointera
   * les diff�rentes composantes du tableau retourn�.
   */
  char *s = (char*) malloc(len);
  strcpy(s, parg);
  
  /**
   * Premier tour de boucle : on compte le nombre d'arguments.
   */
  size_t argc = 0;
  // Un bool�en indiquant si on vient de lire une espace.
  bool space_before = true;
  for (size_t i = 0; i < len; ++i) {
    if (s[i] != ' ' && space_before) {
      argc++;
    }
    space_before = s[i] == ' ';
    // On en profite pour transformer tous les espaces en 0
    if (space_before) {
      s[i] = 0;
    }
  }

  // On peut maintenant allouer le tableau qui sera retourn�
  char ** argv = (char **) malloc(sizeof(char *) * (argc + 1));
  argv[argc] = NULL;
  
  // Deuxi�me tour de boucle : on m�morise le d�but de chaque
  // param�tre
  size_t p = 0;
  // Indique si on est en train de lire un argument
  bool in_arg = false;
  for (size_t i = 0; i < len; ++i) {
    if (s[i] != 0 && !in_arg) {
      argv[p] = &s[i];
      ++p;
    }
    in_arg = s[i] != 0;
  }
  
  return argv;
}

/**
 * Permet de lib�rer la m�moire occup�e par le tableau retourn� par
 * analyse_arg.
 */
void dispose_arg(char *argv[]) {
  free(argv[0]);
  free(argv);
}
