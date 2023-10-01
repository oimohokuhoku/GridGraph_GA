#include "crossover.hpp"
#include "../individual.hpp"
#include "../other/meta_observer.hpp"
#include"../../other/int_queue.hpp"
#include"../../other/random.hpp"
#include<iostream>

void ICrosser::modifyGraph(Individual& indiv) {
	IntQueue nodeQ(numNode() * degree());
	Random random;
	
	for (int n = 0; n < numNode(); ++n) {
		for (int d = 0; d < degree(); ++d) {
			if (indiv.adjacent[n][d] == -1) nodeQ.enqueue(n);
		}
	}

	MetaObserver::calcInheritRate((double)nodeQ.count() / (numNode() * degree()));

	int n1, n2, c;
	while (true) {
		if(nodeQ.isEmpty()) return;
		n1 = nodeQ.dequeue();
		c = 1 + nodeQ.count() * 2;

		while(c > 0){
			if(nodeQ.isEmpty()) return;

			c--;
			n2 = nodeQ.dequeue();

			/* skip for random */
			int r = random.randomInt(0, 2);
			if (r == 1) {
				nodeQ.enqueue(n2);
				continue;
			}

			if (n1 == n2) {
				nodeQ.enqueue(n2);
				continue;
			}

			if (getLength(n1, n2) > length()) {
				nodeQ.enqueue(n2);
				continue;
			}

			if (indiv.haveEdge(n1, n2)) {
				nodeQ.enqueue(n2);
				continue;
			}

			indiv.adjacent[n1][indiv.degrees[n1]] = n2;
			indiv.degrees[n1]++;
			indiv.adjacent[n2][indiv.degrees[n2]] = n1;
			indiv.degrees[n2]++;
			break;
		}
	}
}