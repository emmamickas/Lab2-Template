#include <stdio.h>              // for I/O
#include <stdlib.h>             // for exit()
#include <libgen.h>             // for dirname()/basename()
#include <string.h>             // for string functions

typedef struct node{
        char  name[64];         // node's name string
        char  type;             // type = 'D' or 'F'  
   struct node *child, *sibling, *parent;
}NODE;


NODE *root, *cwd, *start;       // root->/, cwd->CWD, start->start_node
char line[128];                 // user input line
char command[16], pathname[64]; // command pathname strings

// you need these for dividing pathname into token strings
char gpath[128];                // token string holder
char *name[32];                 // token string pointers
int  n;                         // number of token strings

//               0       1      2    
char *cmd[] = {"mkdir", "ls", "quit", 0};

int findCmd(char *command)
{
   int i = 0;
   while(cmd[i]){
     if (strcmp(command, cmd[i])==0)
         return i;
     i++;
   }
   return -1;
}

NODE *search_child(NODE *parent, char *name)
{
  NODE *p;
  printf("search for %s in parent DIR %s\n", name, parent->name);
  p = parent->child;
  if (p==0)
    return 0;
  while(p){
    if (strcmp(p->name, name)==0)
      return p;
    p = p->sibling;
  }
  return 0;
}

int insert_child(NODE *parent, NODE *q)
{
  NODE *p;
  printf("insert NODE %s into parent's child list\n", q->name);
  p = parent->child;
  if (p==0)
    parent->child = q;
  else{
    while(p->sibling)
      p = p->sibling;
    
    p->sibling = q;
  }
  q->parent = parent;
  q->child = 0;
  q->sibling = 0;
}

/***************************************************************
 This mkdir(char *name) makes a DIR in the current directory
 You MUST improve it to mkdir(char *pathname) for ANY pathname
****************************************************************/

int mkdir(char *name)
{
  NODE *p, *q;
  printf("mkdir: name=%s\n", name);

  if (!strcmp(name, "/") || !strcmp(name, ".") || !strcmp(name, "..")){
    printf("can't mkdir with %s\n", name);
    return -1;
  }
  if (name[0]=='/')
    start = root;
  else
    start = cwd;

  printf("check whether %s already exists\n", name);
  p = search_child(start, name);
  if (p){
    printf("name %s already exists, mkdir FAILED\n", name);
    return -1;
  }
  printf("--------------------------------------\n");
  printf("ready to mkdir %s\n", name);
  q = (NODE *)malloc(sizeof(NODE));
  q->type = 'D';
  strcpy(q->name, name);
  insert_child(start, q);
  printf("mkdir %s OK\n", name);
  printf("--------------------------------------\n");
    
  return 0;
}

// This ls() list CWD. You MUST improve it to ls(char *pathname)
int ls()
{
  NODE *p = cwd->child;
  printf("cwd contents = ");
  while(p){
    printf("[%c %s] ", p->type, p->name);
    p = p->sibling;
  }
  printf("\n");
}

int quit()
{
  printf("Program exit\n");
  exit(0);
  // improve quit() to SAVE the current tree as a Linux file
  // for reload the file to reconstruct the original tree
}

int initialize()
{
    root = (NODE *)malloc(sizeof(NODE));
    strcpy(root->name, "/");
    root->parent = root;
    root->sibling = 0;
    root->child = 0;
    root->type = 'D';
    cwd = root;
    printf("Root initialized OK\n");
}

int main()
{
  int index;

  initialize();

  while(1){
     printf("Enter command line [mkdir|ls|quit] : ");
     fgets(line, 128, stdin);     // get input line
     line[strlen(line)-1] = 0;    // kill \n at end
     
     sscanf(line, "%s %s", command, pathname); // extract command pathname
     printf("command=%s pathname=%s\n", command, pathname);
      
     if (command[0]==0) 
         continue;

     index = findCmd(command);

     switch (index){
        case 0: mkdir(pathname); break;
        case 1: ls();            break;
        case 2: quit();          break;
     }
  }
}
