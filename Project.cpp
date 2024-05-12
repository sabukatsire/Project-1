#include<bits/stdc++.h> 
using namespace std;
int n,a[10000000]; // 
struct object{
	int start;
	set<int> accept;
	set<pair<int,pair<int,char> > > edges;
};
object concatenate(object a,object b){
	object c;
	if(a.start > b.start) swap(a,b);
	c.start = a.start;
	set<int>::iterator it;
	set<pair<int,pair<int,char> > >::iterator it1;
	for(it = b.accept.begin(); it != b.accept.end(); it++){
		if((*it) != b.start) c.accept.insert((*it));
	} 
	if(b.accept.count(b.start) == true){
		for(it = a.accept.begin(); it != a.accept.end(); it++){
			c.accept.insert((*it));
		} 
	}
	for(it = a.accept.begin(); it != a.accept.end(); it++){
		for(it1 = b.edges.begin(); it1 != b.edges.end(); it1++){
			if((*it1).first == b.start){ 
				a.edges.insert({(*it),{(*it1).second.first,(*it1).second.second}});
			}
		}
	}
	for(it1 = a.edges.begin(); it1 != a.edges.end(); it1++){
		c.edges.insert((*it1));
	} 
	for(it1 = b.edges.begin(); it1 != b.edges.end(); it1++){
		if((*it1).first != b.start) c.edges.insert((*it1));
	} 
	return c;
}
object unite(object a,object b){
	object c;
	set<int>::iterator it;
	set<pair<int,pair<int,char> > >::iterator it1;
	c.start = min(a.start,b.start);
	if(a.accept.count(a.start) == true || b.accept.count(b.start) == true) c.accept.insert(c.start);
	for(it = a.accept.begin(); it != a.accept.end(); it++){
		if((*it) != max(a.start,b.start)) c.accept.insert((*it));
	} 
	for(it = b.accept.begin(); it != b.accept.end(); it++){
		if((*it) != max(a.start,b.start)) c.accept.insert((*it));
	} 
	for(it1 = a.edges.begin(); it1 != a.edges.end(); it1++){
		if((*it1).first == a.start) c.edges.insert({c.start,{(*it1).second.first,(*it1).second.second}});
		else c.edges.insert((*it1));
	} 
	for(it1 = b.edges.begin(); it1 != b.edges.end(); it1++){
		if((*it1).first == b.start) c.edges.insert({c.start,{(*it1).second.first,(*it1).second.second}});
		else c.edges.insert((*it1));
	} 
	return c;
}
object star(object a){
	object c;
	c.start = a.start;
	c.accept.insert(a.start);
	set<int>::iterator it;
	set<pair<int,pair<int,char> > >::iterator it1;
	for(it = a.accept.begin(); it != a.accept.end(); it++){
		for(it1 = a.edges.begin(); it1 != a.edges.end(); it1++){
			if((*it1).first == a.start){
				c.edges.insert({(*it),{(*it1).second.first,(*it1).second.second}});
			}
		} 
		c.accept.insert((*it));
	} 
	for(it1 = a.edges.begin(); it1 != a.edges.end(); it1++){
		c.edges.insert((*it1));
	} 
	return c;
}
object conversion(string s,int idx){ 
	object curr;
	if(s.size() == 0){
		curr.start = n;
		curr.accept.insert(n);
		n += 1;
		return curr;
	}
	if(s.size() == 1){
		curr.start = n;
		curr.edges.insert({n,{n+1,s[0]}});
		curr.accept.insert(n+1);
		n += 2;
		return curr;
	}
	if(s.size() == 2 && s[1] == '*'){
		curr.start = n;
		curr.edges.insert({n,{n+1,s[0]}});
		curr.edges.insert({n+1,{n+1,s[0]}});
		curr.accept.insert(n);
		curr.accept.insert(n+1);
		n += 2;
		return curr;
	}
	if(s[0] == '(' && s[s.size()-1] == ')' && a[idx] == s.size()-1){
		return conversion(s.substr(1,s.size()-2),idx+1);
	}
	if(s[0] == '(' && s[s.size()-2] == ')' && a[idx] == s.size()-2 && s[s.size()-1] == '*'){
		curr = conversion(s.substr(1,s.size()-3),idx+1);
		curr = star(curr);
		return curr;
	} 
	vector<string> v;
	int last = 0;
	for(int i=0;i<s.size();i++){
		if(s[i] == '|'){
			v.push_back(s.substr(last,i-last));
			last = i+1;
		}
		if(s[i] == '(') i = a[idx+i] + i; 
	} v.push_back(s.substr(last,s.size()-last)); 
	if(v.size() > 1){
		curr = conversion(v[0],idx);
		int now = v[0].size();
		for(int i=1;i<v.size();i++){
			object c = conversion(v[i],idx+now+i);
			curr = unite(curr,c);
			now += v[i].size();
		}
		return curr;
	}
	else if(v.size() == 1){
		int i = 0;
		if(s[0] == '('){ 
			if(s[a[idx]+1] == '*'){
				curr = conversion(s.substr(0,a[idx]+2),idx);
				i = a[idx] + 2;
			}
			else{
				curr = conversion(s.substr(0,a[idx]+1),idx);
				i = a[idx] + 1;
			}
		}
		else{
			if(s[1] == '*'){
				curr = conversion(s.substr(0,2),idx);
				i = 2;
			}
			else{
				curr = conversion(s.substr(0,1),idx);
				i = 1;
			}
		}
		for(i=i;i<s.size();i++){
			if(s[i] == '('){
				if(i + a[idx+i] == s.size()-1){ 
					curr = concatenate(curr,conversion(s.substr(i,a[idx+i]+1),idx+i));
					i = i + a[idx+i];
				}
				else if(s[i+a[idx+i]+1] == '*'){ 
					curr = concatenate(curr,conversion(s.substr(i,a[idx+i]+2),idx+i));
					i = i + a[idx+i] + 1;
				}
				else{
					curr = concatenate(curr,conversion(s.substr(i,a[idx+i]+1),idx+i));
					i = i + a[idx+i];
				}
			}
			else{
				if(i == s.size()-1){
					curr = concatenate(curr,conversion(s.substr(i,1),idx+i));
				}
				else if(s[i+1] == '*'){ 
					curr = concatenate(curr,conversion(s.substr(i,2),idx+i));
					i++;
				}
				else{
					curr = concatenate(curr,conversion(s.substr(i,1),idx+i));
				}
			}
		}
		return curr;
	}            
}
signed main(){
	string s;
	cin>>s;
	stack<int> st;
	for(int i=0;i<s.size();i++){
		if(s[i] == '(') st.push(i);
		else if(s[i] == ')'){
			a[i] = i - st.top();
			a[st.top()] = i - st.top();
			st.pop();
		}
	}
	object fin = conversion(s,0); 
	set<int>::iterator it;
	vector<pair<char,int> > v[n+5];
	set<pair<int,pair<int,char> > >::iterator it1;
	map<int,int> mp;
	set<int> nodes,accepted;
	for(it1 = fin.edges.begin(); it1 != fin.edges.end(); it1++){
		int from = (*it1).first, to = (*it1).second.first;
		nodes.insert(from); nodes.insert(to);
	} int idx = 0;
	for(it = nodes.begin(); it != nodes.end(); it++){
		mp[(*it)] = idx; idx++;
	} 
	for(it1 = fin.edges.begin(); it1 != fin.edges.end(); it1++){
		int from = (*it1).first, to = (*it1).second.first;
		v[mp[from]].push_back({(*it1).second.second,mp[to]});
	}
	for(it = fin.accept.begin(); it != fin.accept.end(); it++){
		accepted.insert(mp[(*it)]);
	} 
	cout<<"Total number of nodes: "<<nodes.size()<<endl;
	cout<<"Total number of acceptor nodes: "<<accepted.size()<<endl;
	cout<<"Total number edges: "<<fin.edges.size()<<endl;
	cout<<"Acceptor nodes: ";
	for(it = accepted.begin(); it != accepted.end(); it++){
		cout<<(*it)<<" ";
	} cout<<endl;
	for(int i=0;i<nodes.size();i++){
		cout<<"Node "<<(i+1)<<" has "<<v[i].size()<<" outgoing edges";
		if(v[i].size() == 0) cout<<"."<<endl;
		else cout<<":"<<endl;
		for(int j=0;j<v[i].size();j++){
			cout<<"Connected to "<<v[i][j].second<<" by "<<v[i][j].first<<endl;
		} 
	}
}
