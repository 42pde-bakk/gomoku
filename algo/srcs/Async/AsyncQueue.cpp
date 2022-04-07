//
// Created by Peer De bakker on 4/7/22.
//

#include "AsyncQueue.hpp"

AsyncQueue<Gamestate*>&	getOutputQueue() {
	static AsyncQueue<Gamestate*>	outputQueue;

	return (outputQueue);
}
