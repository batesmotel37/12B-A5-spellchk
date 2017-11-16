// $Id: hashset.c,v 1.198 2014-03-15 10:44:33-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugf.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %d, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void free_hashset (hashset *this) {
   for(size_t c=0;c<this->size;c++){
      while(this->chains[c]!=NULL){
         hashnode *t=this->chains[c]->link;
         free(this->chains[c]->word);
         free(this->chains[c]);
         this->chains[c]=t;
      }
   }
   free(this->chains);
   free(this);
   DEBUGF ('h', "free (%p)\n", this);
}

void put_hashset (hashset *this, const char *item) {
   size_t index=strhash(item)%this->size;
   for(hashnode *curr=this->chains[index];curr!=NULL;curr=curr->link){
      if(strcmp(item,curr->word)==0)
         return;
   }
   hashnode *insrt=malloc(sizeof(struct hashnode));
   assert(insrt!=NULL);
   insrt->word=strdup(item);
   assert(insrt->word!=NULL);
   insrt->link=this->chains[index];
   this->chains[index]=insrt;
   this->load+=1;
   if(this->load*2>this->size){
      size_t old_size=this->size;
      size_t new_size=old_size*2+1;
      hashnode **newchains=calloc(new_size,
                                  sizeof(hashnode *));
      assert(newchains!=NULL);
      for(size_t c=0;c<old_size;c++){
         while(this->chains[c]!=NULL){
            size_t new_index=strhash(this->chains[c]->word)%new_size;
            hashnode *t=this->chains[c]->link;
            this->chains[c]->link=newchains[new_index];
            newchains[new_index]=this->chains[c];
            this->chains[c]=t;
         }
      }
      free(this->chains);
      this->chains=newchains;
      this->size=new_size;
   }
}

bool has_hashset (hashset *this, const char *item) {
   size_t index=strhash(item)%this->size;
   for(hashnode *curr=this->chains[index];curr!=NULL;curr=curr->link){
      if(strcmp(item,curr->word)==0)
         return true;
   }
   return false;
}

void dump_hashset(hashset *this){
   size_t no_words=0;
   fprintf(stdout,"%6d words in the hash set\n",(int)this->load);
   fprintf(stdout,"%6d size of the hash array\n",(int)this->size);
   for(int chain_length=1;no_words<this->load;chain_length++){
      int chains_of_given_length=0;
      for(size_t c=0;c<this->size;c++){
         int this_chain_length=0;
         for(hashnode *curr=this->chains[c];curr!=NULL
             ;curr=curr->link){
            this_chain_length+=1;
         }
         if(this_chain_length==chain_length){
            chains_of_given_length+=1;
         }
      }
      if(chains_of_given_length>0){
         no_words+=(chains_of_given_length*chain_length);
         fprintf(stdout,"%6d chains of size %d\n",
                        chains_of_given_length,chain_length);
      }
   }
}

void cluster_hashset(hashset *this){
   for(size_t c=0;c<this->size;c++){
      if(this->chains[c]!=NULL){
         fprintf(stdout,"array[%10d] = %12u \"%s\"\n",(int)c,
                        (int)strhash(this->chains[c]->word),
                        this->chains[c]->word);
         for(hashnode *curr=this->chains[c]->link;curr!=NULL;
                                                  curr=curr->link){
               for(int i=0;i<18;i++)
                  fprintf(stdout," ");
               fprintf(stdout,"= %12u \"%s\"\n",
                              (int)strhash(curr->word),curr->word);
         }
      }
   }
}

