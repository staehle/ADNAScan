#ifndef ADNA_HPP
#define ADNA_HPP

#define MEMKEY "/adna_table"

typedef unordered_map<std::string, adna::ReadPair> readmap;

struct _table {
   pid_t PID;
   int   readsAssigned;
   int   readsComplete;
};


#endif
