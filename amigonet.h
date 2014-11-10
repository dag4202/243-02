/**
 * amigonet.h
 * $Revision: 1.10 $
 * James Heliotis
 *
 * AmigoNet
 * A CSci 243 project that focuses on memory allocation and data structures
 *
 * The idea is a simulation of a social network. New users can register,
 * and friends ("amigos") can be added or removed. in addition, one can
 * find out how many degrees of separation there are between any two
 * users. (See http://en.wikipedia.org/wiki/Six_Degrees_of_Kevin_Bacon
 * for an example in a different context.)
 */

#ifndef AMIGONET_H
#define AMIGONET_H

// The friend data structure (student choice)
typedef struct Friends_struct *Friends;

/**
 * The public user structure
 */
typedef struct User_struct {
    const char *name;
    Friends amigos;
} User;

/**
 * Initialize the system.
 * (This is where some memory is initially allocated.)
 */
void create_amigonet();

/**
 * Shut down the system.
 * (This is where a bunch of memory is freed.)
 */
void destroy_amigonet();

/**
 * Add a new user with initially no friends.
 * The parameter 'name' is use to initialize the new user entry.
 * Note: it must be copied to prevent the caller from changing
 * the name later.
 * If the name already exists, then this function does nothing.
 */
void addUser( const char *name );

/**
 * Locate a user structure using the user's name as a key.
 * User structures are needed for the addAmigo, removeAmigo,
 * and separation functions.
 * If the user does not exist, NULL is returned.
 */
User *findUser( const char *name );

/**
 * Add a friend (the "amigo") to the user. This should be a two-way
 * addition. If the two users are already friends, this function
 * does nothing.
 */
void addAmigo( User *user, User *amigo );

/**
 * "Un-friend" two users. This is, again, a two-way operation.
 * If the two users were not friends, this function does nothing.
 */
void removeAmigo( User *user, User *ex_amigo );

/**
 * Determine the minimum degree of separation between two users.
 * A user is degree 0 from him/her self. If user a is degree n from
 * user b, then user a is at most degree n+1 from user b's friends.
 * If the users are not connected by a chain of friends, this
 * function returns -1.
 */
size_t separation( const User *user1, const User *user2 );

/**
 * For each user, print on standard output the user's name, and a list
 * of that user's friends.
 * The function outputs an empty line at the beginning and at the end.
 * FORMAT OF EACH LINE:
 * User <username>; friends: <friendname1> ... <friendnameN>
 * 
 */
void dump_data();

#endif
