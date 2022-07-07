#pragma once
#include <vector>
#include <deque>
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server)
        : search_server_(search_server)
        , empty_requests_(0)
        , current_time_(0) 
        {
    }
 
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate); 
    
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status); 
    std::vector<Document> AddFindRequest(const std::string& raw_query); 
    int GetNoResultRequests() const;
   
private:
    struct QueryResult 
    {
        uint64_t times;
        int results;
    };
   std::deque<QueryResult> requests_;
    const SearchServer& search_server_;
    int empty_requests_;
    uint64_t current_time_;
    const static int min_in_day_ = 1440;
 
    void Add(int results_num); 
    
};

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) 
   {
        SearchServer search_server_;
        const auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        Add(result.size());
        return result;
    }