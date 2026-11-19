#include "queue.h"

using namespace std;

void Queue::push(const string& v) { l.pushTail(v); }

string Queue::pop() {
    if (l.size() == 0) fail("queue is empty");
    return l.popHead();
}

Strs Queue::items() const { return l.items(); }

void Queue::load(const Strs& t) { for (int i = 0; i < (int)t.size(); i = i + 1) push(t[i]); }

string Queue::show() const {
    if (l.size() == 0) return "(empty queue)";
    return "front -> " + join(l.items(), ", ") + " <- back";
}
