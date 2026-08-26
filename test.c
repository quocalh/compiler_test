#include <stdio.h>
#include "assert.h"
#include <string.h>

#include "misc/uthash.h"

struct my_struct {
    int id;            /* we'll use this field as the key */
    char name[10];
    UT_hash_handle hh; /* makes this structure hashable */
};


struct my_struct *users = NULL;    /* important! initialize to NULL */
void add_user(int user_id, char *name) 
{
  struct my_struct *s;

  HASH_FIND_INT(users, &user_id, s);  /* id already in the hash? */
  if (s == NULL) {
    s = (struct my_struct *)malloc(sizeof *s);
    s->id = user_id;
    HASH_ADD_INT(users, id, s);  /* id: name of key field */
  }
  strcpy(s->name, name);
}

void add_user_local(struct my_struct **users, int user_id, char *name) 
{
  struct my_struct *s;

  HASH_FIND_INT(*users, &user_id, s);  /* id already in the hash? */
  if (s == NULL) {
    s = (struct my_struct *)malloc(sizeof *s);
    s->id = user_id;
    HASH_ADD_INT(*users, id, s);  /* id: name of key field */
  }
  strcpy(s->name, name);
}

struct my_struct* find_user_local(struct my_struct **users, int user_id)
{
  struct my_struct* s;
  HASH_FIND_INT(*users, &user_id, s);
  return s;
}

void delete_user_local(struct my_struct** users, struct my_struct* del_user)
{
  HASH_DEL(*users, del_user);
  free(del_user);
}

void clear_user_local(struct my_struct** users)
{
  HASH_CLEAR(hh, *users);
  // users gonna be none
}

int main()
{
  printf("hello world\n");
 
  struct my_struct *hash_table = NULL;
  add_user_local(&hash_table, 4, "neckhurt");

}

