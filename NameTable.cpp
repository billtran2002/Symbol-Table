// NameTable.cpp Bill Tran UID: 505604257

// This is a correct but inefficient implementation of
// the NameTable functionality.

#include "NameTable.h"
#include <string>
#include <vector>
#include <functional>
#include <cassert>
using namespace std;

// This class does the real work of the implementation.


//NameTableImpl FUNCTION DEFINITIONS
class NameTableImpl
{
public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    vector<int> m_ids;
    struct identifiers
    {
        int line_ID, depth_ID;
        string str_ID;
    };
    vector<identifiers> Buckets[10000];
    int depthTracker = 0;
};


void NameTableImpl::enterScope()
{
    m_ids.push_back(-1);
    depthTracker = depthTracker + 1;
}

bool NameTableImpl::exitScope()
{
    while (m_ids.empty() == 0 && m_ids.back() != -1)
    {
        Buckets[m_ids.back()].pop_back();
        m_ids.pop_back();
    }
    if (m_ids.empty())
    {
        return false;
    }
    m_ids.pop_back();
    depthTracker = depthTracker - 1;
    return true;
}

bool NameTableImpl::declare(const string& id, int lineNum)
{
    //Hash Function for Strings
    unsigned int h = 2166136261u;
    for (int c = 0; c != id.size(); c++)
    {
        h += id[c];
        h *= 16319;
    }
    int key = h % 10000;

    identifiers currentID;
    currentID.depth_ID = depthTracker;
    currentID.line_ID = lineNum;
    currentID.str_ID = id;

    if (Buckets[key].size() == 0)
    {
        Buckets[key].push_back(currentID);
        m_ids.push_back(key);
        return true;
    }
    while (Buckets[key].front().str_ID != id)
    {
        key = key + 1;
        if (key == 10000)
        {
            key = 0;
        }
        if (Buckets[key].size() == 0)
        {
            break;
        }
    }
    if (Buckets[key].size() != 0 && depthTracker <= Buckets[key].back().depth_ID)
    {
        return false;
    }
    Buckets[key].push_back(currentID);
    m_ids.push_back(key);
    return true;
}

int NameTableImpl::find(const string& id) const
{
    if (id.empty())
        return -1;

    //Hash Function for Strings
    unsigned int h = 2166136261u;
    for (int c = 0; c != id.size(); c++)
    {
        h += id[c];
        h *= 16319;
    }
    int key = h % 10000;

    if (Buckets[key].size() == 0)
    {
        return -1;
    }

    else
    {
        while (Buckets[key].size() != 0 && Buckets[key].front().str_ID != id)
        {
            key = key + 1;
            if (key == 10000)
            {
                key = 0;
            }

        }
        if (Buckets[key].size() == 0)
        {
            return -1;
        }
        return Buckets[key].back().line_ID;
    }
}

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
