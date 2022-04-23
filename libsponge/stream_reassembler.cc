#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}
#include <iostream>
//using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), first_unread(0),
    first_unassembled(0),first_unacceptable(0), _eof(false), unassembled_bytes_cnt(0){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
/*void merge_unorderdata(size_t listindex,size_t mergeindex, string mergedata){
	string listdata = unorder_data[listindex];
	if(listindex < mergeindex){
		string tmp = mergedata.substr(listdata.size() - (mergeindex - listindex));
		listdata += tmp;
	}else {


	}

}
void merge_unoderdata(){
	sort(unorder_index.begin(),unorder_index.end());
	for(auto it = unorder_index.begin(); i < unorder_index.end(); i++){
		
	}
}*/
void StreamReassembler::check_if_need_merge(size_t index,const string& data){
	/*auto it = unorder_data.lower_bound(index);
	if(it != unorder_data.end() && it->first != index){// find lower bound and not itself ,check if need merge
		size_t lowindex = it->first;
		string lowdata = it->second;
		if(lowindex + lowdata.size() >= index ){
			if(index + data.size() > lowindex + lowdata.size()){
				lowdata += data.substr(lowindex + lowdata.size() - index);
				unorder_data.erase(index);//merge with lower
				unorder_data[lowindex] = lowdata;//is  this neccessary??????????????????
				check_if_need_merge(lowindex, lowdata);//after modify recursive check;
			}else{
				unorder_data.erase(index);

			}
			
		}
	 }*/
	 //else {
	 	//cout << "ERROR,has no lowerbound!" << endl;
	 //}

	 auto it = unorder_data.upper_bound(index);
	 if(it != unorder_data.end()){
	 	size_t upindex = it->first;
	 	string updata = it->second;
	 	if(index + data.size() >= upindex ){
	 		if(upindex + updata.size() > index + data.size()){
	 			string newdata = data;
				newdata += updata.substr(index + data.size() - upindex);
				unorder_data[index] = newdata;
	 			unorder_data.erase(upindex);//merge with higher
	 			check_if_need_merge(index, newdata);//after modify recursive check;
	 		}else{
	 			if(index == 1)
	 				cout << "erase upindex = " << upindex <<"upindex data = "<<updata <<"index =" << index << "data = "<< data <<  endl;
	 			unorder_data.erase(upindex);
	 			
	 		}
	 		
	 	}
	 }
	for(it = unorder_data.begin(); it != unorder_data.end(); it++){
	 	size_t lowindex = it->first;
		string lowdata = it->second;
		if(lowindex >= index){
			return;
		}
		else if(lowindex + lowdata.size() >= index ){
			if(index + data.size() > lowindex + lowdata.size()){
				lowdata += data.substr(lowindex + lowdata.size() - index);
				unorder_data.erase(index);//merge with lower
				unorder_data[lowindex] = lowdata;//is  this neccessary??????????????????
				check_if_need_merge(lowindex, lowdata);//after modify recursive check;
			}else{
				unorder_data.erase(index);

			}
			
		}
	 }

}
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    //int len = min(data.size(), capacity - _output.buffer_size());
   /* if(index == first_unacceptable){
    	int len = _output.write(data);
    	first_unacceptable += len;
    	if(unorder_index.front() == first_unacceptable){
    		len = _output.write(unorder_data[first_unacceptable]);
    		
    		unorder_index.pop_front();
    		unorder_data.erase(first_unacceptable);

    		first_unacceptable += len;
    	}else if (unorder_index.front() < first_unacceptable){
    		
    	}

	}else if(index > first_unacceptable){
		if(index > unorder_index.back()){
			unorder_index.push_back(index);
			unorder_data[index] = data;
		}else if(index < unorder_index.front() ){
			if(index + data.size() > unorder_index.front()){

				string tmp = unorder_data[unorder_index.front()];
				unorder_index.pop_front();
				unorder_index.push_front(index);
				
			}
		}
		for(auto it = unorder_index.begin(); it != unorder_index.end(); it++){
			if(index <  )
		}
	
	}else if(index < first_unacceptable)
	*/
	first_unread = _output.bytes_read();
	first_unacceptable = first_unread + _capacity;
	if(index >= first_unacceptable){
		return;
	}
	_eof = eof || _eof;
	if(data == ""){
		if(_eof && empty()){// if enpty string cannot just return.need check eof
			_output.end_input();
		}
		return;
	}
	size_t len;
	
	if(index == first_unassembled){

    	len = _output.write(data);
    	first_unassembled += len;
    	

	}else if(index > first_unassembled){
		auto it = unorder_data.find(index);
		if(it != unorder_data.end()){ //find same index , data change to the max one 
			string samedata = it->second;
			if(data.size() > samedata.size()){
				unorder_data[index] = data;
				//unassembled_bytes_cnt += data.size();
				check_if_need_merge(index, data);
			}// smaller do nothing
		}else{// has no same index ,just insert 
			unorder_data[index] = data;
			//unassembled_bytes_cnt += data.size();
			check_if_need_merge(index, data);
		}
		return;// has not write ,just return

	}else if(index < first_unassembled){
		if(index + data.size() > first_unassembled){
			 len = _output.write(data.substr(first_unassembled - index));
			 first_unassembled += len;
		}
	}

	bool ret = false;
	for(auto it = unorder_data.begin();it != unorder_data.end();it++){
    	size_t firstindex = it->first;
    	string firstdata = it->second;
    	if(firstindex == first_unassembled){
    		ret = write_output(firstindex,firstdata);
    		//check_if_datasize_too_big(firstindex,firstdata);
    		//len = _output.write(firstdata);
    		//unassembled_bytes_cnt -= firstdata.size();
    		unorder_data.erase(firstindex);
    		//first_unassembled += len;
    		if(ret)
    			break;
    	}else if (firstindex < first_unassembled){
    		if(firstindex + firstdata.size() <= first_unassembled ){
    			unorder_data.erase(firstindex);
    		}else{
    			//check_if_datasize_too_big(firstindex,firstdata);
    			//len = _output.write(firstdata.substr(first_unassembled - firstindex));
    			string str = firstdata.substr(first_unassembled - firstindex);
    			ret = write_output(firstindex,str);
    			unorder_data.erase(firstindex);
    			//first_unassembled += len;
    			if(ret)
    				break;

    		}
    	}else{
    		break;
    	}
	}
    if(_eof && empty()){
    	_output.end_input();
    }
}
bool StreamReassembler::check_if_datasize_too_big(size_t index,string& data){
	//return;
	size_t bufsize = _output.remaining_capacity();
    if(data.size() > bufsize){
    	unorder_data[index + bufsize] = data.substr(bufsize);
    	return true;
    }
    return false;
}
bool StreamReassembler::write_output(size_t index, string& data){
	bool ret = check_if_datasize_too_big(index, data);
	size_t len = _output.write(data);
	first_unassembled += len;
	return ret;
}

size_t StreamReassembler::unassembled_bytes() const { 
	//return _capacity - _output.buffer_size(); 
	//return unassembled_bytes_cnt;
	int ret =  0;
	for(auto it = unorder_data.begin(); it != unorder_data.end(); it++){
		ret += it->second.size();
	}
	return ret;
}

bool StreamReassembler::empty() const { return unorder_data.empty(); }
