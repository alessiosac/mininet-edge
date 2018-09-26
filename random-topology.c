#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define True         1
#define False        0
typedef int bool;

int ran( int k);

int pick_to( int* closed, int vertices )
{
	int  vertex;
	if ( closed == NULL )       /* pick any vertex, even if picked before... */
		do {
			vertex = ran( vertices );
		} while ( vertex == 0 ); /* ...except for start */
	else {
		do {
			vertex = ran( vertices );
		} while ( closed[ vertex ] );
		closed[ vertex ] = True;
	}
	return ( vertex );
}

int unpicked_vertices_p( int* closed, int n )
{
	int  i = 1;

	while ( i < n )
		if ( !closed[ i ] )
			return ( True );
		else
			i++;
	return ( False );
}

bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

/*** ran, etc. ***/
void seed_ran( void )
{
	srand( ( unsigned short ) time( NULL ) );
}

/* Return a random integer between 0 and k-1 inclusive. */
int ran( int k )
{
	return rand() % k;
}

int main(int argc, char **argv)
{
	int N, E, H, i, j, index, selected_sw;
	int MaxBW;
	FILE *fptr;
	char str[100];
	char *ptr;
	char *ipAddress;

	int  start = 0,
	     finish, from, to, edge_count = 0,
	     *adj_matrix,
	     *closed_list;

    if (argc < 6) // no arguments were passed
    {
        printf("error: missing command line arguments\n");
        printf("usage: random-topology N E H MaxBw Controller_IP\n");
        return 1;
    }
    else
    {
        N           = strtol (argv[1],&ptr,10);
        E           = strtol (argv[2],&ptr,10);
        H           = strtol (argv[3],&ptr,10);
        MaxBW       = strtol (argv[4],&ptr,10);
        ipAddress   = argv[5];
    }

    if(!isValidIpAddress(ipAddress))
    {
        printf("Please insert a valid IP address\n");
        return 1;
    }


	seed_ran();
	printf("Number of switches: %d\n", N);
	printf("Number of links:%d\n", E);
	printf("Number of hosts:%d\n", H);
	printf("Maximum Bandwidth:%d\n", MaxBW);
	printf("Controller IP:%s\n", ipAddress);

	fptr=fopen("test_topology.py", "w");
	if(!fptr) return 1;
	fptr=fopen("test_topology.py", "w");
	if(!fptr) return 1;

	finish=N-1;
	if (N==1) {
		printf("SingleVertexNetwork\n");
		return 0;
	}

	/* adjacency matrix to represent graph */
	if((adj_matrix=(int *)calloc(N * N, sizeof(int)))==NULL) {
		printf("InsufficientStorage\n");
		return 0;
	}

	/* closed_list[ i ] == True if vertices[ i ] is on some path from start to finish */
	if((closed_list=(int *)calloc(N, sizeof(int)))==NULL) {
		printf("InsufficientStorage\n");
		free(adj_matrix);
		return 0;
	}

	/* put each vertex on some path from start to finish */
	closed_list[ start ] = True;
	do {
		/* from start to some vertex not already picked */
		to = pick_to(closed_list, N);
		adj_matrix[ to ] = ran( MaxBW ) + 1;
		edge_count++;

		/* from current vertex to vertex != finish */
		if ( to != finish ) {
			from = to;
			closed_list[ finish ] = False;
			while ( ( to = pick_to( closed_list, N ) ) != finish ) {
				adj_matrix[ from * N + to ] = ran( MaxBW ) + 1;
				edge_count++;
				from = to;
			}

			/* from vertex to finish */
			adj_matrix[ from * N + finish ] = ran( MaxBW ) + 1;
			edge_count++;
		}
	} while ( unpicked_vertices_p( closed_list, N ) );

	/* add additional edges as needed */
	while ( edge_count < E ) {
		from = ran( N - 1 );
		to = pick_to( NULL, N );
		if ( !adj_matrix[ from * N + to ]  &&  from != to ) {
			adj_matrix[ from * N + to ] = ran( MaxBW ) + 1;
			edge_count++;
		}
	}

	fprintf(fptr, "#!/usr/bin/python\n");
	fprintf(fptr, "from mininet.net import Mininet\n");
	fprintf(fptr, "from mininet.node import Controller, RemoteController, OVSKernelSwitch, UserSwitch\n");
	fprintf(fptr, "from mininet.cli import CLI\n");
	fprintf(fptr, "from mininet.log import setLogLevel\n");
	fprintf(fptr, "from mininet.link import Link, TCLink\n");
	fprintf(fptr, "\n\ndef topology():\n");
	fprintf(fptr, "    net = Mininet(controller=RemoteController, link=TCLink, switch=OVSKernelSwitch)\n");
	//fprintf(fptr, "  c0 = net.addController('c0', controller=Controller, ip='127.0.0.1', port=6633 )\n");
	fprintf(fptr, "    c0 = net.addController( 'c0', controller=RemoteController, ip='%s', port=6633 )\n", ipAddress);
	//fprintf(fptr, "  s1 = net.addSwitch('s1', listenPort=6673, mac='00:00:00:00:00:01')\n");
	//fprintf(fptr, "  h1 = net.addHost('h1', mac='00:00:00:00:00:01', ip='10.0.0.1/24')\n");
	//fprintf(fptr, "  h2 = net.addHost('h2', mac='00:00:00:00:00:02', ip='10.0.0.2/24')\n");
	//fprintf(fptr, "  net.addLink(s1, h1)\n");
	//fprintf(fptr, "  net.addLink(s1, h2)\n");

	for(i=1; i<=N; i++){
		sprintf(str, "    s%d = net.addSwitch('s%d')", i, i);
		fprintf(fptr, "%s\n", str);
	}

	for(i=1; i<=H; i++){
		sprintf(str, "    h%d = net.addHost('h%d')", i, i);
		fprintf(fptr, "%s\n", str);
		selected_sw=(rand() % N) + 1;
		sprintf(str,"    net.addLink(s%d, h%d, bw=%d)", selected_sw, i, MaxBW);
		fprintf(fptr, "%s\n", str);
	}

	for(i=1; i<N;i++) {
		for (j=i+1;j<=N;j++) {
			index=(i-1)*N + j-1;
			if(adj_matrix[index]){
				//printf("%d %d %d\n", i, j, adj_matrix[index]);
				sprintf(str,"    net.addLink(s%d, s%d, bw=%d)", i, j, adj_matrix[index]);
				fprintf(fptr, "%s\n", str);
			}
		}
	}

	fprintf(fptr, "    net.build()\n");
	fprintf(fptr, "    c0.start()\n");
	for(i=1; i<=N; i++){
		sprintf(str, "    s%d.start([c0])", i);
		fprintf(fptr, "%s\n", str);
	}
	fprintf(fptr, "    CLI(net)\n");
	fprintf(fptr, "    net.stop()\n");

	fprintf(fptr, "\n\nif __name__ == '__main__':\n");
	fprintf(fptr, "    setLogLevel('info')\n");
	fprintf(fptr, "    topology()\n");
	fclose(fptr);
	/* free storage */
	free( adj_matrix );
	free( closed_list );
	return 0;
}