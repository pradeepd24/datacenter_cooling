#include <stdio.h>
#include <stdlib.h>

// structure of a point in the datacenter array
struct point {
	int i,j; // co-ordinates of a point
	int value;
	int visited;
	int reachcount; // tells how many different ways this point can be reached from its neighbors
};

struct point **datacenter, *start;
int W, H, NumPaths, TotalCount;

void Initialise(struct point *p) {  //updates reachcount of point and increments totalcount
	if(p->value == 2)
		start = p;
	if(p->value != 1)
		TotalCount++;

	if(p->i+1 < H && datacenter[p->i+1][p->j].value != 1)
		p->reachcount++;
	if(p->i-1 >= 0 && datacenter[p->i-1][p->j].value != 1)
		p->reachcount++;
	if(p->j+1 < W && datacenter[p->i][p->j+1].value != 1)
		p->reachcount++;
	if(p->j-1 >= 0 && datacenter[p->i][p->j-1].value != 1)
		p->reachcount++;
}


void GetInputs(char *input_filename) { // gets input and calls initialise for each point
	FILE *fp;
	int i,j;
	fp=fopen(input_filename,"r");

	fscanf(fp,"%d %d", &W, &H);
	datacenter = (struct point **)malloc(H*sizeof(struct point *));

	for(i=0; i<H; i++) {
		datacenter[i] = (struct point *)malloc(W*sizeof(struct point));
		for(j=0; j<W; j++) {
			fscanf(fp, "%d", &(datacenter[i][j].value));
			datacenter[i][j].i = i;
			datacenter[i][j].j = j;
			datacenter[i][j].visited = 0;
			datacenter[i][j].reachcount = 0;
		}	
	}

	for(i=0; i<H; i++)
		for(j=0; j<W; j++)
			Initialise(&datacenter[i][j]);

	fclose(fp);  
}


void reupdatereachcount(int cond[], int i, int j) { // reupdate reachcount of neighbors
	if(cond[0])
		datacenter[i+1][j].reachcount++;
	if(cond[1])
		datacenter[i-1][j].reachcount++;
	if(cond[2])
		datacenter[i][j+1].reachcount++;
	if(cond[3])
		datacenter[i][j-1].reachcount++;
}

void Visit(struct point p, int CurCount) {
	int c1=0,c2=0; // variables to check if the neighbors must be visited
	int cond[4]={0, 0, 0, 0}; // variables that helps to see if the neighbors reachcount is modified
	if(p.value == 3 && CurCount == TotalCount) {
		NumPaths++;
		return;
	}

	if(p.value == 3)
		return;
 
	// decrementing reachcount of neighbors, because it no longer(from next time) can be reached via this point
	if(p.i+1 < H && datacenter[p.i+1][p.j].value != 1 && datacenter[p.i+1][p.j].visited == 0) {
		datacenter[p.i+1][p.j].reachcount--;
		cond[0] = 1;
	}
 
	if(p.i-1 >= 0 && datacenter[p.i-1][p.j].value != 1 && datacenter[p.i-1][p.j].visited == 0) {
		datacenter[p.i-1][p.j].reachcount--;
		cond[1] = 1;
	}

	if(p.j+1 < W && datacenter[p.i][p.j+1].value != 1 && datacenter[p.i][p.j+1].visited == 0) {
		datacenter[p.i][p.j+1].reachcount--;
		cond[2] = 1;
	}

	if(p.j-1 >= 0 && datacenter[p.i][p.j-1].value != 1 && datacenter[p.i][p.j-1].visited == 0) {
		datacenter[p.i][p.j-1].reachcount--;
		cond[3] = 1;
	}

	if( p.i+1 < H) {
		c1 = datacenter[p.i+1][p.j].visited == 0 && datacenter[p.i+1][p.j].value == 0 && datacenter[p.i+1][p.j].reachcount <= 1;
		c2 = datacenter[p.i+1][p.j].visited == 0 && datacenter[p.i+1][p.j].value == 3 && datacenter[p.i+1][p.j].reachcount < 1;
		if(c1||c2) { // have to visit (i+1,j)
			datacenter[p.i+1][p.j].visited = 1;
			Visit(datacenter[p.i+1][p.j], CurCount+1);
			datacenter[p.i+1][p.j].visited = 0;
			reupdatereachcount(cond, p.i, p.j);
			return;
		}
	}  

	if( p.i-1 >= 0) {
		c1 = datacenter[p.i-1][p.j].visited == 0 && datacenter[p.i-1][p.j].value == 0 && datacenter[p.i-1][p.j].reachcount <= 1;
		c2 = datacenter[p.i-1][p.j].visited == 0 && datacenter[p.i-1][p.j].value == 3 && datacenter[p.i-1][p.j].reachcount < 1;
		if(c1||c2) { // have to visit (i-1,j)
			datacenter[p.i-1][p.j].visited = 1;
			Visit(datacenter[p.i-1][p.j], CurCount+1);
			datacenter[p.i-1][p.j].visited = 0;
			reupdatereachcount(cond, p.i, p.j);
			return;
		}
	} 

	if( p.j+1 < W) {
		c1 = datacenter[p.i][p.j+1].visited == 0 && datacenter[p.i][p.j+1].value == 0 && datacenter[p.i][p.j+1].reachcount <= 1;
		c2 = datacenter[p.i][p.j+1].visited == 0 && datacenter[p.i][p.j+1].value == 3 && datacenter[p.i][p.j+1].reachcount < 1;
		if(c1||c2) { // have to visit (i,j+1)
			datacenter[p.i][p.j+1].visited = 1;
			Visit(datacenter[p.i][p.j+1], CurCount+1);
			datacenter[p.i][p.j+1].visited = 0;
			reupdatereachcount(cond, p.i, p.j);
			return;
		}
	}

	if( p.j-1 >= 0) {
		c1 = datacenter[p.i][p.j-1].visited == 0 && datacenter[p.i][p.j-1].value == 0 && datacenter[p.i][p.j-1].reachcount <= 1;
		c2 = datacenter[p.i][p.j-1].visited == 0 && datacenter[p.i][p.j-1].value == 3 && datacenter[p.i][p.j-1].reachcount < 1;
		if(c1||c2) { // have to visit (i,j-1)
			datacenter[p.i][p.j-1].visited = 1;
			Visit(datacenter[p.i][p.j-1], CurCount+1);
			datacenter[p.i][p.j-1].visited = 0;
			reupdatereachcount(cond, p.i, p.j);
			return;
		}
	} 
 
	// if none of the above 4 cases, then visit all the 4 neighbors that are eligible
	if(p.i+1 < H && datacenter[p.i+1][p.j].value != 1 && datacenter[p.i+1][p.j].visited == 0) {
		datacenter[p.i+1][p.j].visited = 1;
		Visit(datacenter[p.i+1][p.j], CurCount+1);
		datacenter[p.i+1][p.j].visited = 0;
	}

	if(p.i-1 >= 0 && datacenter[p.i-1][p.j].value != 1 && datacenter[p.i-1][p.j].visited == 0) {
		datacenter[p.i-1][p.j].visited = 1;
		Visit(datacenter[p.i-1][p.j], CurCount+1);
		datacenter[p.i-1][p.j].visited = 0;
	}

	if(p.j+1 < W && datacenter[p.i][p.j+1].value != 1 && datacenter[p.i][p.j+1].visited == 0) {
		datacenter[p.i][p.j+1].visited = 1;
		Visit(datacenter[p.i][p.j+1], CurCount+1);
		datacenter[p.i][p.j+1].visited = 0;
	}

	if(p.j-1 >= 0 && datacenter[p.i][p.j-1].value != 1 && datacenter[p.i][p.j-1].visited == 0) {
		datacenter[p.i][p.j-1].visited = 1;
		Visit(datacenter[p.i][p.j-1], CurCount+1);
		datacenter[p.i][p.j-1].visited = 0;
	}
 
	// increment reachcount of the neighbors while leaving this point
	reupdatereachcount(cond, p.i, p.j);
}


void Dealloc(void) {
	int i;
	for(i=0; i<H; i++)
		free(datacenter[i]);
	free(datacenter);
}

int main(int argc, char **argv) {
	GetInputs(argv[1]);
	start->visited = 1;
	Visit(*start,1);
	printf("%d\n",NumPaths);
	Dealloc();
 	return 1;
}
 
