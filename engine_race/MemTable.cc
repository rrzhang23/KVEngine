#include "MemTable.h"
#include <mutex>
using std::string;
namespace polar_race {

std::mutex MemTable::table_mtx;
std::mutex MemTable::immut_mtx;
MemTable* MemTable::_mutable = nullptr;
MemTable* MemTable::immut = nullptr;

MemTable* MemTable::getMemtable() {
    std::lock_guard<std::mutex> guard(table_mtx);
    if (_mutable == nullptr) {
        // set new memtable
        _mutable = new MemTable();
    }
    return _mutable;
}

MemTable* MemTable::getImmut() {
    return immut;
}

void MemTable::setImmutable() {
    std::lock_guard<std::mutex> guard(table_mtx);

    if (this->_immut) return;
    this->_immut = true;

    immut_mtx.lock();
    immut = _mutable;
    // call immut collect thread
    immut_mtx.unlock();

    _mutable = new MemTable();
    
}

RetCode MemTable::_write(const PolarString& key, const PolarString& value) {
    std::lock_guard<std::mutex> guard(_write_mtx);
    string _key = key.ToString();
    if (index.find(_key) != index.end()) {
        ERROR("MemTable::_write() Existed key.");
        return _update(key, value);
    }
    else {
        string* new_v = new string(value.data());
        index.insert(std::pair<string, string*>(_key, new_v));
        size += value.size();
        size += key.size();
        if (size > MEMTABLE_MAX_SIZE) 
            setImmutable();
    }
    return RetCode::kSucc;
}

RetCode MemTable::_update(const PolarString& key, const PolarString& value) {
    string _key = key.ToString();
    if (index.find(_key) == index.end()) {
        ERROR("MemTable::_update() Unexisted key.");
        return RetCode::kNotFound;
    }
    else {
        string* new_v = new string(value.data());
        string* old_v = index.find(_key)->second;
        size -= old_v->size();
        delete old_v;
        index.find(_key)->second = new_v;
        size += value.size();
    }
    return RetCode::kSucc;
}

bool MemTable::contains(const PolarString& key) {
    return index.find(key.ToString()) != index.end();
}

RetCode MemTable::write(const PolarString& key, const PolarString& value) {
    if (contains(key))
        return _update(key, value);
    else 
        return _write(key, value);
}   





} // namespace polar_race