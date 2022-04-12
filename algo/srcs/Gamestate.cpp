//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include "Threadpool.hpp"
#include "Job.hpp"
#include <cassert>
#include <algorithm>
#include "Gomoku.hpp"
#include <fstream>
#include <sstream>
#include <chrono>

bool g_log = false;

Gamestate::Gamestate(const Gamestate &x) :
		Heuristic(x), moves(x.moves),
		parent(&x), children()
{ }

Gamestate::~Gamestate() {
	for (auto& child : this->children) {
		delete child;
		child = nullptr;
	}
	this->children.clear();
}

bool compareGamestates(const Gamestate* a, const Gamestate* b) { return (*a < *b); }
bool compareGamestatesReverse(const Gamestate* a, const Gamestate* b) { return (*b < *a); }

// https://core.ac.uk/download/pdf/33500946.pdf
void Gamestate::generate_children() {
//	const static std::string	root = "/code/log/";
	const static std::string	root = "/Users/pde-bakk/PycharmProjects/gomoku/algo/log/";
#if THREADED
	static std::fstream fs(root + "generate_children.txt", std::fstream::out | std::fstream::trunc);
	static AsyncQueue<Job>&			jobQueue(getJobQueue());
	static Threadpool& threadpool(Threadpool::GetInstance());
	assert(fs.is_open());
#else
	static std::fstream fs(root + "generate_children_singlethreaded.txt", std::fstream::out | std::fstream::trunc);
#endif
	static compareFunc compareFuncs[] = {
		compareGamestates, compareGamestatesReverse
	};
	if (!this->children.empty() || this->has_winner()) {
		return ;
	}
	auto start = std::chrono::steady_clock::now();
	auto current_time = std::chrono::steady_clock::now();
	long long int elapsed_time = 0;
	if (this->board.none()) {
		int idx = 20 * 9 + 9;
		auto	*middle = new Gamestate(*this);
		middle->place_stone(idx);
		this->children.emplace_back(middle);
		return ;
	}
	Bitboard	empty_neighbours(this->get_empty_neighbours());
	if (empty_neighbours.none()) {
		throw std::runtime_error("Error. No more empty tiles");
	}

	fs << "Starting to place stones\n";
	unsigned int stones = 0;
	for (unsigned int i = 0; i < REALBOARDSIZE; i++) {
		if (!empty_neighbours.bitboard_get(i) || Bitboard::isSeperatingBitIndex(i))
			continue;
		++stones;
#if THREADED
		auto loop_start = std::chrono::steady_clock::now();

		Job job(this, i);
		jobQueue.push(job);

		current_time = std::chrono::steady_clock::now();
		elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - loop_start).count();
		fs << "enqueueing Job took " << elapsed_time << " microseconds.\n";
#else
		auto	*child = new Gamestate(*this);
		child->place_stone(i);
		this->children.emplace_back(child);
#endif
	}
#if THREADED
	{
		current_time = std::chrono::steady_clock::now();
		auto c = std::chrono::duration_cast<std::chrono::microseconds>(current_time - start).count();
		fs << "Main thread took " << c << " microseconds to get to threadpool.WaitForWorkers().\n";
		auto a = std::chrono::steady_clock::now();
		jobQueue.waitTillFinished();
		threadpool.WaitForWorkers();
		current_time = std::chrono::steady_clock::now();
		elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - a).count();
		fs << "Main thread had to wait " << elapsed_time << " microseconds.\n";
	};

#endif
	assert(!children.empty());
	std::sort(children.begin(), children.end(), compareFuncs[this->get_player()]);

	current_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - start).count();
	fs << "Gamestate::place_stone took " << elapsed_time << " microseconds to place " << stones << " stones.\n\n";
}

void	Gamestate::write_to_file() const {
	static int idx = 1;
	std::stringstream ss;
	ss << "/Users/pde-bakk/PycharmProjects/gomoku/algo/tests/log/gamestate_" << idx++;
	std::fstream fs(ss.str(), std::fstream::out | std::fstream::trunc);

	if (!fs.is_open())
		throw std::runtime_error("Couldn't write to logfile");
//	fs << Heuristic::hash_fn(this->board) << '\n';
	fs << "Heuristic value: " << this->h << '\n';
	print_board(fs, false);
}

Gamestate::Gamestate() { }

void Gamestate::place_stone(unsigned int move_idx) {
//	static std::atomic<int> whoo = 1;
	assert(move_idx < BOARDSIZE);
	assert (this->tile_is_empty(move_idx));
//
//	dprintf(1, "whoo = %zu\n", whoo);
//	whoo++;

	this->set(move_idx, this->get_player());
	this->moves.emplace_back(move_idx, player);
	if (!this->perform_captures(move_idx)) {
		// check double threes
		// It is important to note that it is not forbidden to introduce
		// a double-three by capturing a pair.
	}
	// TODO: update heuristic value
	this->set_h();
	this->add_h_for_captures();
//	this->write_to_file();
	this->depth++;
	this->change_player();
}

int Gamestate::change_player() {
	this->player = !this->player;
	return (this->player);
}

const Move &Gamestate::get_first_move() const {
	return this->moves.front();
}

void Gamestate::clear_children() {
	for (auto child : this->children) {
		delete child;
	}
	this->children.clear();
}

void Gamestate::print_history(std::ostream& o) const {
	if (this->parent)
		this->parent->print_history(o);
	this->print_board(o, true);
	o << '\n';
}

void Gamestate::add_child(Gamestate *child) {
	this->children.push_back(child);
}
