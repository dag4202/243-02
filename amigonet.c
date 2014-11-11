#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "hashTable.h"
#include "queue.h"



struct Friends_struct {
	HashTable userAmigos;	
	unsigned int amigoCount;
};


#include "amigonet.h"
typedef struct user_graph {
	HashTable users;
	unsigned int userCount;
} *UserGraph;


static UserGraph graph;

/**
* Uses Cyclic Redundancy Check (CRC to create a unique hash value based on 
* individual characters in the key given and the size of the hashTable.
*/
unsigned int CRCHash(const void *key, unsigned int size){
	const char *tmp = (const char *)key;
	int Bytes;
	char *name = (char *)malloc((Bytes=strlen(key)+1));
	strcpy(name, tmp);
	uint8_t result = 0;
	uint8_t divisor = 0xD5; // standard for CRC-8
	
	for (int i = 0; i<Bytes; ++i){
		result ^=(name[i]); //also initializes result to first letter
		for(uint8_t letter = 8; letter>0; --letter){
			if(result & 128){ //if first bit in result is 1
				result = (result << 1)^divisor;// divisor can divide
			} else {
				result = (result << 1); //divisor can't divide it
			}
		}
		
	}
	unsigned int hash = (unsigned int) result;
	free(name);
	return ((hash*hash) %size);
	
}

void create_amigonet(){
	graph = (UserGraph)malloc(sizeof(struct user_graph));
	graph->users = ht_create(CRCHash);
	graph->userCount = 0;
}
/**
* Frees a user and their amigos
*/
static void destroy_User(User *user){
	assert(user!=0);
	if(user->amigos!=0){
		ht_destroy(user->amigos->userAmigos);
		free(user->amigos);
	}
	free(user);
}

/**
* Finds all users and frees them.
*/
static void destroy_Users(){
	char **names = (char **)ht_keys(graph->users);
	User *user;
	for (unsigned int i = 0; i<ht_count(graph->users); i++){
		user = (User *)ht_get(names[i], graph->users);
		destroy_User(user);
		free(names[i]);
	}
	free(names);
}



void destroy_amigonet(){
	assert(graph!=0);
	if (graph->users!=0){
		if (graph->userCount!=0){
			destroy_Users();
		}
		ht_destroy(graph->users);
	}
	free(graph);
}

void addUser( const char *name ){
	if(graph->users!=0 && graph->userCount!=0){
		char **names = (char **)ht_keys(graph->users);
		for(unsigned int i = 0; i< graph->userCount; i++){
			if (strcmp(name, names[i])==0){
				return;
			}
		}
		free(names);
	}
	char *UserName = malloc(sizeof(strlen(name)+1));
	strcpy(UserName, name);
	User *person = (User *)malloc(sizeof(struct User_struct));
	person->name = UserName;
	person->amigos = (Friends)malloc(sizeof(struct Friends_struct));
	Friends friends = person->amigos;
	friends->userAmigos = ht_create(CRCHash);
	friends->amigoCount = 0;
	ht_add((void *)person->name, (void *)person, graph->users);
	graph->userCount+=1;
}

User *findUser( const char *name ){
	if(graph->users!=0 && graph->userCount!=0){
		char **names = (char **)ht_keys(graph->users);
		for(unsigned int i = 0; i< graph->userCount; i++){
			if (strcmp(name, names[i])==0){
				void *key = (void *) names[i];
				free(names);
				return (User *)ht_get(key, graph->users);
			}
		}
		free(names);
	}
	return NULL;
}

void addAmigo(User *user, User *amigo){
	Friends friends = user->amigos;
	if(ht_add((void *)amigo->name, (void *)amigo, friends->userAmigos)){
		friends->amigoCount+=1;
	}
	friends = amigo->amigos;
	if(ht_add((void *)user->name, (void *)user, friends->userAmigos)){
		friends->amigoCount+=1;
	}
}
void removeAmigo( User *user, User *ex_amigo ){
	Friends friends = user->amigos;
	if (ht_remove( (void *)ex_amigo->name, friends->userAmigos)!= NULL){
		friends->amigoCount-=1;
	}
	friends = ex_amigo->amigos;
	if (ht_remove((void *)user->name, friends->userAmigos)){
		friends->amigoCount-=1;
	}
}

/**
*Encapsulates a user to include degree of seperation from a source.
*/
typedef struct queue_node {
	User *user;
	size_t distance;
} *AmigoNode;

/**
* The init method for an AmigoNode. Allocates memory.
*/
static AmigoNode amig_create( User *user, size_t distance){
	AmigoNode node = (AmigoNode) malloc(sizeof(struct queue_node));
	node->user = user;
	node->distance = distance;
	return node;
}

size_t separation( const User *user1, const User *user2 ){
	AmigoNode check;
	User *user = (User *)ht_get((void *)user1->name,graph->users);
	Friends adjacent;
	char **names;
	size_t seperation = 0;
	Queue queue = que_create();
	HashTable visited = ht_create(CRCHash);
	AmigoNode toVisit = amig_create(user, seperation);

	ht_add((void *)user->name, ( void *) user, visited);
	que_push((void *) toVisit, queue);

	while(!que_isEmpty(queue)){
		if((check = (AmigoNode)que_pop(queue))->user == user2){
			seperation = check->distance;
			while(!que_isEmpty(queue)){
				free(que_pop(queue));
			}
			free(check);
			ht_destroy(visited);
			que_destroy(queue);
			return seperation;
		}
		adjacent = check->user->amigos;
		seperation = check->distance + 1;
		free(check);
		if(adjacent->amigoCount>0){
			names = (char **)ht_keys(adjacent->userAmigos);
			for(unsigned int i = 0; i< adjacent->amigoCount; i++){
				if (!ht_contains((void *)names[i], visited)){
					user = (User *) ht_get((void *) names[i], 
						adjacent->userAmigos);
					ht_add(( void *)names[i], ( void *)user, visited);
					toVisit = amig_create(user, seperation);
					que_push((void *) toVisit, queue);  
				}
			}
			free(names);
		}
	}
	que_destroy(queue);
	ht_destroy(visited);
	return -1;
}

void dump_data(){
	printf("\n");
	if(graph->userCount!=0){
		char **names = (char **)ht_keys(graph->users);
		char **amigos;
		User *user;
		printf("\n");
		for (unsigned int i = 0; i<graph->userCount; i++){
			user = (User *)ht_get((void *) names[i], graph->users);
			printf("User %s; friends:", user->name);
			amigos = (char **)ht_keys(user->amigos->userAmigos);
			for (unsigned int j=0; j < user->amigos->amigoCount; j++){
				printf(" %s", amigos[j]);
			}
			free(amigos);
			printf("\n");
		}
	free(names);
	}
	printf("\n");

}
