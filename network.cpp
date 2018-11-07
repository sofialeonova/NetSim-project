#include "network.h"
#include "random.h"

void Network::resize(const size_t& size) {
	values.resize(size);
	RNG.normal(values,0,1);
	}
	
bool Network::add_link(const size_t& a, const size_t& b) {
	std::vector<size_t> neighbors_ = neighbors(a);
	if((a < size()) and (b < size()) and (a != b)) {
		 if (std::find(neighbors_.begin(),neighbors_.end(),b) == neighbors_.end()) {
			 links.insert({{a,b},{b,a}});
		     return true;
		 } else {return false;}
	} 
	return false;
}  


size_t Network::random_connect(const double& mean_) {
	links.erase(links.begin(),links.end());   //we erase all the links
	std::vector<size_t> linking_nodes; 
	std::vector <int> degrees_(size()); 
	RNG.poisson(degrees_,mean_);   //how many links we need to do per node
	
	for(size_t m(0);m < size();++m) {  //table with linking nodes
		linking_nodes.push_back(m);
	}
		
	for (size_t i(0);i < size();++i) {  //we iterate on the links' keys
		RNG.shuffle(linking_nodes);  //we shuffle the table with linking nodes randomly for each key   
           for(size_t a(0);a < size();++a) { //we create links if conditions are respected:
			  if(degree(i) < size_t(degrees_[i]) and (size_t(degrees_[linking_nodes[a]]) > degree(linking_nodes[a]))) {  
				     // 1. node i can still create links
					 // 2. we check if node [linking_nodes[a]] can still do other links
			            add_link(i,linking_nodes[a]);   
			  }                            
	       }
     } 
	return links.size();
}

size_t Network::set_values(const std::vector<double>& new_values) {
     if(new_values.size()<=size()) {
		 for(size_t n(0); n < new_values.size();++n) {
			 values[n] = new_values[n];
			 }
		return new_values.size();
		 }
	 for (size_t i(0); i < size(); ++i) {
		 values[i] = new_values[i];
		 }
     return size(); 
}

size_t Network::size() const {
	return values.size();
}
	
size_t Network::degree(const size_t &_n) const {
	return links.count(_n);
}

double Network::value(const size_t& _n) const {
	return values[_n];
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> sort_val = values;
	std::sort(sort_val.begin(),sort_val.end(),std::greater<double>());
	return sort_val;
	
}

std::vector<size_t> Network::neighbors(const size_t& _n) const {
	std::vector<size_t> neighbors_;
	for (auto link : links) {
			if (link.first == _n) {
				neighbors_.push_back(link.second);
			}
	}
	return neighbors_;
}



