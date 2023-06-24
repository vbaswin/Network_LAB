/*
The `fd_set` is a data structure used by the `select()` function to represent a set of
file descriptors. It is essentially a bit array where each bit corresponds to a file descriptor.

The `fd_set` data structure is typically implemented as an array of fixed size, where each element in the array
corresponds to a file descriptor. The size of the `fd_set` is determined by the constant `FD_SETSIZE`, which represents
the maximum number of file descriptors that can be stored in a set.

Here's a simplified definition of the `fd_set` structure:

*/
typedef struct {
	unsigned long fds_bits[FD_SETSIZE / (sizeof(unsigned long) * 8)];
} fd_set;

/*

The `fd_set` structure contains an array called `fds_bits`, which holds the bits representing the file descriptors.
The size of the `fds_bits` array is calculated based on the maximum number of file descriptors (`FD_SETSIZE`). Each
bit in `fds_bits` corresponds to a file descriptor, and a value of `1` indicates that the file descriptor is a member
of the set, while a value of `0` indicates it is not.

To manipulate the `fd_set` and operate on individual file descriptors within it, you can use the following macros:

==--------------------------------------------------------------------------------------------===
- `FD_ZERO(fd_set *set)`: Clears all file descriptors in the set.
- `FD_SET(int fd, fd_set *set)`: Adds the file descriptor `fd` to the set.	<---
- `FD_CLR(int fd, fd_set *set)`: Removes the file descriptor `fd` from the set.
- `FD_ISSET(int fd, fd_set *set)`: Checks if the file descriptor `fd` is a member of the set.
==-------------------------------------------------------------------------------------------====

These macros help manage the file descriptor sets by setting and clearing the corresponding bits in the `fds_bits` array.

Here's an example of how to use the `fd_set` and these macros:

*/
#include <stdio.h>
#include <sys/select.h>

int main() {
	int fd1, fd2;
	fd_set read_fds;

	FD_ZERO(&read_fds);
	FD_SET(fd1, &read_fds);
	FD_SET(fd2, &read_fds);

	if (FD_ISSET(fd1, &read_fds)) {
		printf("fd1 is ready for reading\n");
	}

	if (FD_ISSET(fd2, &read_fds)) {
		printf("fd2 is ready for reading\n");
	}

	return 0;
}

/*
In this example, `fd1` and `fd2` are two file descriptors of interest. The `FD_ZERO()` macro initializes the `read_fds`
set, `FD_SET()` adds the file descriptors `fd1` and `fd2` to the set, and `FD_ISSET()` checks if a file descriptor is
ready for reading.

The `fd_set` data structure and associated macros provide a convenient way to manipulate and check file descriptor sets
when using the `select()` function for I/O multiplexing in socket programming.
*/
