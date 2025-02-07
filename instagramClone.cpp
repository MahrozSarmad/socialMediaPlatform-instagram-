#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <Windows.h>

using namespace std;

struct namingNode {

	string name;
	namingNode* left;
	namingNode* right;
	namingNode(string name) {
		this->name = name;
		left = nullptr;
		right = nullptr;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

string getCurrentTime() {
	auto currentTime = chrono::system_clock::now();
	time_t timeNow = chrono::system_clock::to_time_t(currentTime);
	ostringstream oss;
	oss << put_time(localtime(&timeNow), "%Y-%m-%d %H:%M:%S");
	string storedTime = oss.str();
	return storedTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct messageNode {
	string content;
	string timestamp;
	messageNode* next;

	messageNode(string msg) {
		content = msg;
		timestamp = getCurrentTime();
		next = nullptr;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct notifications {
	string notification;
	string timeStamp;
	notifications* next;
	notifications(string notification) {
		this->notification = notification;
		timeStamp = getCurrentTime();
		next = nullptr;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct posts {
	string description;
	string timeDate;
	posts* next;
public:
	// this is a linked list based implementation of posts for each user
	posts(string description) {
		this->description = description;
		timeDate = getCurrentTime();
		next = nullptr;
	}
	void printPost() {
		cout << "-----------------" << endl;
		cout << "The Post Was Added On " << timeDate << endl;
		cout << "Content: " << description << endl;
		cout << "-----------------" << endl;
	}
};

// create a new post and return that post

posts* createNewPost() {
	string description;
	cout << "Enter New Post Description : ";
	getline(cin, description);
	string timeAndDate = getCurrentTime();
	posts* newpost = new posts(description);
	return newpost;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////




struct friends {
	string userName;
	string relationShipStatus;
	messageNode* messageHead;
	friends* next;

	friends(string name,string status) {
		userName = name;
		relationShipStatus = status;
		next = nullptr;
		messageHead = nullptr;
	}
	void addMessage(string msg) {
		messageNode* newMessage = new messageNode(msg);
		newMessage->next = messageHead;
		messageHead = newMessage;
	}

	void displayMessages() {
		if (!messageHead) {
			cout << "No messages with " << userName << " yet." << endl;
			return;
		}
		cout << "Chat with " << userName << ":" << endl;
		messageNode* temp = messageHead;
		while (temp) {
			cout << "[" << temp->timestamp << "] " << temp->content << endl;
			temp = temp->next;
		}
	}
};


void updateRoot(namingNode* root,string &oldName,string &newName) {
	// updating the root
	if (root == nullptr) {
		return;
	}
	if (root->name == oldName) {
		root->name = newName;
		cout << "Updated" << endl;
		return;
	}
	updateRoot(root->left, oldName, newName);
	updateRoot(root->right, oldName, newName);
}

bool checkIfAlreadyNameExist(namingNode* root, string name) {
	if (root == nullptr) {
		return false;
	}
	else if (name > root->name) {
		return checkIfAlreadyNameExist(root->right, name);
	}
	else if (name < root->name) {
		return checkIfAlreadyNameExist(root->left, name);
	}
	else {
		cout << "Couldn't Update the name as the name you entered is Already in Use" << endl;
		return true;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct node {
	string name;
	string password;
	string city;
	friends* friendHead;
	posts* postHead; // for saving posts
	string activeLast;
	notifications* notificationHead;
	node* next;

	// post how to

	node(string name, string password, string city, string activeLast) {
		this->name = name;
		this->password = password;
		this->city = city;
		this->activeLast = activeLast;
		friendHead = nullptr;
		next = nullptr;
		postHead = nullptr;
		notificationHead = nullptr;

	}

	void insertNotification(string str) {
													// insert like stack 
		notifications* n = new notifications(str);
		if (notificationHead == nullptr) {
			notificationHead = n;
		}
		else {
			n->next = notificationHead;
			notificationHead = n;
		}
	}
	void printNotifications() {
		cout << "-------------NOTIFICATIONS--------------" << endl;
		notifications* temp = notificationHead; 
		if (temp == nullptr) {
			cout << "No notifications to display." << endl;
			return;
		}

		while (temp != nullptr) {
			cout << "Notification: " << temp->notification << endl;
			cout << "Time: " << temp->timeStamp << endl;
			cout << "---------------------------------------" << endl;
			temp = temp->next; 
		}
	}

	void printInfo() {
		cout << "User Information:" << endl;
		cout << "-----------------" << endl;
		cout << "Name: " << name << endl;
		cout << "Password: " << password << endl;
		cout << "City: " << city << endl;
		cout << "Last Active: " << activeLast << endl;
		cout << "-----------------" << endl;
	}

	void changeName(node* userList,namingNode* root) {
		cout << "-----------------" << endl;
		string oldName = name;
		cout << "Enter New Name : ";
		getline(cin, name);
	
		// check if the new name already used in the root
		if (checkIfAlreadyNameExist(root, name)) {
			name = oldName;
			return;
		}
		cout << "Name has updated successfully...." << endl;
		cout << "-----------------" << endl;
		updateRoot(root,oldName,name);
		
		// change the name in the friend list

		node* temp = userList;

		friends* tempFriends = friendHead;

		while (tempFriends) {
			// traverse the friends and see actice relations
			if (tempFriends->relationShipStatus == "Active" || tempFriends->relationShipStatus == "Pending" || tempFriends->relationShipStatus == "PENDING") {
				string nameOfFriend = tempFriends->userName;
				while (temp) {
					// find the name in the user's list
					if (temp->name == nameOfFriend) {

						friends* friendKiFriendList = temp->friendHead;
						while (friendKiFriendList) {
							if (friendKiFriendList->userName == oldName) {
								friendKiFriendList->userName = name;
								break;
							}
						}
						if (tempFriends->relationShipStatus == "Active") {
							temp->insertNotification(oldName + " has updated their name as " + name);
						}
						break;
					}
					temp = temp->next;
				}
			}
			tempFriends = tempFriends->next;
		}





		// update the naming tree
		Sleep(500);
	}
	void changePassword() {
		cout << "-----------------" << endl;
		cout << "Enter New Password : ";
		cin >> password;
		cout << "Password has updated successfully...." << endl;
		cout << "-----------------" << endl;
		Sleep(500);
	}

	// stack implement
	void addNewPost(node* userList) {
		posts* newPost = createNewPost();
		if (postHead == nullptr) {
			postHead = newPost;
		}
		else {
			newPost->next = postHead;
			postHead = newPost;
		}

		// sending notification to all the friends

		friends* tempFriend = friendHead;
		while (tempFriend) {
			if (tempFriend->relationShipStatus == "Active") {
				node* friendNode = userList;
				while (friendNode) {
					if (friendNode->name == tempFriend->userName) {
						// Add a notification
						friendNode->insertNotification(name + " created a new post ");
					}
					friendNode = friendNode->next;
				}
			}
			tempFriend = tempFriend->next;
		}
	}

	void addNewFriend(string name,string status) {
		friends* newFriend = new friends(name, status);
		if (friendHead == nullptr) {
			friendHead = newFriend;
		}
		else {
			// stack ( new request on top )
			newFriend->next = friendHead;
			friendHead = newFriend;
		}
	}

	void viewFriends() {
		if (friendHead == nullptr) {
			cout << "NO FRIENDS EXISTS.." << endl;
			
		}
		else {
			cout << "-------Friends List--------" << endl;
			friends* temp = friendHead;
			while (temp) {
				if (temp->relationShipStatus == "Active") {
					cout << temp->userName << ", ";
				}
				temp = temp->next;
			}
		}

	}
	void remove(string friendName,node* userList) {
		
		friends* prev = nullptr;
		friends* curr = friendHead;

		while (curr) {
			if (curr->userName == friendName) {
				// Remove the friend from the current user's list
				if (prev) {
					prev->next = curr->next;
				}
				else {
					friendHead = curr->next;
				}
				delete curr;
				
				break;
			}
			prev = curr;
			curr = curr->next;
		}
			
		//vice verssa


		node* friendNode = userList;
		while (friendNode) {
			if (friendNode->name == friendName) {
				friends* friendPrev = nullptr;
				friends* friendCurr = friendNode->friendHead;

				while (friendCurr) {
					if (friendCurr->userName == name) { 
						
						if (friendPrev) {
							friendPrev->next = friendCurr->next;
						}
						else {
							friendNode->friendHead = friendCurr->next;
						}
						delete friendCurr;
						break;
					}
					friendPrev = friendCurr;
					friendCurr = friendCurr->next;
				}
				break;
			}
			friendNode = friendNode->next;
		}

	}

	bool acceptFriendRequests(string name,string status) {
		friends* temp = friendHead;
		while (temp) {
			if (temp->userName == name && temp->relationShipStatus == status) {
				temp->relationShipStatus = "Active";	
				return true;
			}
			temp = temp->next;
		}
		return false;
	}
	 
	///////////////////////////////////////////////////////////////////////////////////////////////////

	int getFriendsPosts(posts* friendsPosts[],node* userList) {
		int count = -1;

		friends* tempFriend = friendHead;
		while (tempFriend) {
			if (tempFriend->relationShipStatus == "Active") {
				// Find the friend's node
				node* tempUser = userList; 
				while (tempUser) {
					if (tempUser->name == tempFriend->userName) {
						posts* tempPost = tempUser->postHead;
						while (tempPost) {
							friendsPosts[++count] = tempPost;
							tempPost = tempPost->next;
						}
						break;
					}
					tempUser = tempUser->next;
				}
			}
			tempFriend = tempFriend->next;
		}

		return count; // Return the total number of posts collected
	}

	void sortPostsByTimestamp(posts* friendsPosts[], int count) {
		for (int i = 0; i < count - 1; i++) {
			for (int j = 0; j < count - i - 1; j++) {
				if (friendsPosts[j]->timeDate < friendsPosts[j + 1]->timeDate) {
					// Swap posts
					posts* temp = friendsPosts[j];
					friendsPosts[j] = friendsPosts[j + 1];
					friendsPosts[j + 1] = temp;
				}
			}
		}
	}

	void displayFriendsFeed(node* userList) {
		posts* friendsPosts[100]; // Array to store posts
		int count = getFriendsPosts(friendsPosts,userList);

		// Sort posts by timestamp
		sortPostsByTimestamp(friendsPosts, count);

		// Display the posts
		cout << "------- FRIENDS FEED -------" << endl;
		if (count == -1) {
		cout << "-  NO POSTS FROM FRIENDS  -" << endl;
		}
		for (int i = 0; i <= count; i++) {
			friendsPosts[i]->printPost();
		}
		cout << "----------------------------" << endl;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////

	void showPending() {
		cout << "PENDING REQUESTS : ";
		friends* temp = friendHead;
		while (temp) {
			if (temp->relationShipStatus == "Pending") {
				cout << temp->userName << ", ";
			}
			temp = temp->next;
		}
	}
	void printAllPosts() {
		if (postHead == nullptr) {
			cout << "-----------------" << endl;
			cout << "User Hasn't Posted Anything Yet...." << endl;
			cout << "-----------------" << endl;
			return;
		}

		posts* printingPosts = postHead;
		while (printingPosts) {
			printingPosts->printPost();
			printingPosts = printingPosts->next;
		}
	}
};

void menuForUser() {
	cout << "----- MANAGE PROFILE ------" << endl;
	cout << "---------------------------" << endl;
	cout << "Please choose an option:" << endl;
	cout << "1. Change Name" << endl;
	cout << "2. Change Password" << endl;
	cout << "3. Add Post" << endl;
	cout << "4. View Posts" << endl;
	cout << "5. View Profile Information" << endl;
	cout << "6. View Friends Feed" << endl;
	cout << "7. Back to Main Menu" << endl;
	cout << "---------------------------" << endl;
}

void selectOptions(node* user,node* userList, namingNode* root) {
	while (true) {
		menuForUser();
		int menuNumber;
		cout << "Enter Choice : ";
		cin >> menuNumber;
		cin.ignore();
		
		if (menuNumber == 1) {
			system("CLS");
			// change name (update existed name)
			user->changeName(userList,root);
		}
		else if (menuNumber == 2) {
			system("CLS");
			user->changePassword();
		}
		else if (menuNumber == 3) {
			system("CLS");
			user->addNewPost(userList);
		}
		else if (menuNumber == 4) {
			system("CLS");
			user->printAllPosts();
		}
		else if (menuNumber == 5) {
			system("CLS");
			user->printInfo();
		}
		else if (menuNumber == 6) { // View Friends Feed
			system("CLS");
			user->displayFriendsFeed(userList);
		}
		else if (menuNumber == 7) {
			cout << "Wait ... Back to Main Menu..." << endl;
			Sleep(1000);
			break;
		}
		else {
			system("CLS");
			cout << "INVALID CHOICE" << endl;
			cin.ignore();
			cin.clear();
		}
		user->activeLast = getCurrentTime();  // updating the time
		
	}
}

bool IsUserExists(string& name, node*& userList) {

	node* temp1 = userList;
	while (temp1) {
		if (temp1->name == name) {
			return true;
		}
		temp1 = temp1->next;
	}
	return false;
}


void friendsMenu() {
	cout << "-----Manage Friends ------" << endl;
	cout << "--------------------------" << endl;
	cout << "1. Add Friends" << endl;
	cout << "2. Accept Friends" << endl;
	cout << "3. Remove Friends" << endl;
	cout << "4. View friends" << endl;
	cout << "5. Chat" << endl;
	cout << "6. Notifications" << endl;
	cout << "7. Back to Main Menu" << endl;
	cout << "--------------------------" << endl;
}

bool ifAlreadyFriends(string& name, friends* frndptr) {
	friends *temp = frndptr;
	while (temp) {
		if (name == temp->userName && temp->relationShipStatus== "Active") {
			return true;
		}
		temp = temp->next;
	}
	return false;
}

bool ifAlreadyPending(string& name, friends* frndPtr) {
	friends* temp = frndPtr;
	while (temp) {
		if (name == temp->userName && temp->relationShipStatus == "PENDING") {
			cout << "The Request is Already Pending...Can't Send Request Again.." << endl;
			return true;
		}
		if (name == temp->userName && temp->relationShipStatus == "Pending") {
			cout << "They Have Already Sent You A Request" << endl;
			return true;
		}

		temp = temp->next;
	}
	return false;
}

bool checkRequest(string name, friends*& friendList) {
	friends* temp = friendList;
	while (temp) {
		if (temp->userName == name) {
			return false;
		}
		temp = temp->next;
	}
	return true;
}

void manageFriendShips(node*& user, node*& userList) {
	while (true) {
		
		friendsMenu();
		int options = 0;
		cout << "Enter Choice : ";
		cin >> options;

		if (options == 1) {  // Add Friend Option
			cout << "--------Registered Accounts Names-------" << endl;
			node* temp = userList;
			while (temp) {
				if (temp->name == user->name) {
					temp = temp->next;
					continue;
				}
				cout << temp->name << ", ";
				temp = temp->next;
			}
			cin.ignore();
			cout << endl;
			cout << "Enter Name : ";
			string name;
			getline(cin, name);

			if (IsUserExists(name, userList)) {
				cout << "Searching..." << endl;
				Sleep(500);

				if (ifAlreadyFriends(name, user->friendHead)) {
					cout << "You Are Already Friends With Them" << endl;
					Sleep(1000);
					system("CLS");
					continue;
				}
				if (ifAlreadyPending(name, user->friendHead)) {
				
					Sleep(1000);
					system("CLS");
					continue;
				}

				user->addNewFriend(name,"PENDING");



				// Vice versa: Add a pending request to the other user's list
				node* tempUser = userList;
				while (tempUser) {
					if (tempUser->name == name) {
						tempUser->addNewFriend(user->name,"Pending");// Add current user's name as "Pending"
						tempUser->insertNotification(user->name + " sent you a friend request ");
						break;
					}
					tempUser = tempUser->next;
				}

				cout << "Friend request sent successfully!" << endl;
			}
			else {
				cout << "No Account With Such UserName Found" << endl;
			}
		}
		else if (options == 2) {
			cin.ignore();
			user->showPending();
			cout << endl;
			cout << "Do You Want To Cancel Or Accept The Request?" << endl;
			string userChoice;
			cout << "Enter (accept or cancel) : ";
			cin >> userChoice;
			cin.ignore();
			if (userChoice == "cancel") {
				cout << "Enter Name : ";
				string name;
				getline(cin, name);
				user->remove(name, userList);
				cout << "You have Cancelled " << name << " from your pending Requests." << endl;
				continue;
			}
			cout << "Enter Name : ";
			string name;
			getline(cin, name);
			////////////////////////////////////////////////
			if (checkRequest(name, user->friendHead)) {
				cout << "No Such Request Found..." << endl;
				Sleep(1000);
				system("CLS");
				continue;
			}
			////////////////////////////////////////////////
			if (!user->acceptFriendRequests(name, "Pending")) {
				system("CLS");
				Sleep(500);
				cout << "No Such Request Found..." << endl;
				continue;
			}

			cout << endl;

			 //vice versa
			
			node* tempUser = userList;
			while (tempUser) {
				if (tempUser->name == name) {
					tempUser->acceptFriendRequests(user->name,"PENDING"); 
					tempUser->insertNotification(user->name + " accepted your friend request.");
					break;
				}
				tempUser = tempUser->next;
			}
			
		}
		else if (options == 3) {
			// Block friend
			cin.ignore();
			user->viewFriends();
			cout << endl;
			string name;
			cout << "Enter Name: ";
			getline(cin, name);
			user->remove(name, userList); // Pass the users list to the block function
			cout << "You have Remove " << name << " from your Friends." << endl;
		}

		else if (options == 4) {
			user->viewFriends();
		}
		else if (options == 5) { // Chat Option
			cout << "Enter the friend's name you want to chat with: ";
			cin.ignore();
			string friendName;
			getline(cin, friendName);

			// Find the friend in the user's friend list
			friends* friendPtr = user->friendHead;
			while (friendPtr && friendPtr->userName != friendName) {
				friendPtr = friendPtr->next;
			}

			if (!friendPtr || friendPtr->relationShipStatus != "Active") {
				cout << "No active friendship found with " << friendName << "." << endl;
				continue;
			}

			// Chat Menu
			while (true) {
				cout << "Chat with " << friendName << ":" << endl;
				cout << "1. Send Message" << endl;
				cout << "2. View Messages" << endl;
				cout << "3. Exit Chat" << endl;
				cout << "Enter choice: ";
				int chatOption;
				cin >> chatOption;
				cin.ignore();

				if (chatOption == 1) {
					cout << "Enter your message: ";
					string message;
					getline(cin, message);

					// Add the message to both users' stacks
					friendPtr->addMessage("You: " + message);

					// Mirror the message in the other user's friend list
					node* tempUser = userList;
					while (tempUser) {
						if (tempUser->name == friendName) {
							friends* otherUserFriend = tempUser->friendHead;
							while (otherUserFriend && otherUserFriend->userName != user->name) {
								otherUserFriend = otherUserFriend->next;
							}
							if (otherUserFriend) {
								otherUserFriend->addMessage(user->name + ": " + message);
								tempUser->insertNotification("New message from " + user->name);
							}
							break;
						}
						tempUser = tempUser->next;
					}
					cout << "Message sent!" << endl;
				}
				else if (chatOption == 2) {
					friendPtr->displayMessages();
				}
				else if (chatOption == 3) {
					break;
				}
				else {
					cout << "Invalid option. Try again!" << endl;
					cin.clear();
					cin.ignore();
				}
			}
		}
		else if (options == 6) {
			system("CLS");
			user->printNotifications();
		}
		else if (options == 7) {
			cout << "Wait.... Back to Main Menu" << endl;
			break;
		}
		else {
			cout << "Invalid Option. Try Again!" << endl;
			cin.clear();
			cin.ignore();
			
		}
		Sleep(1000);
		system("CLS");
	
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

namingNode* insert(string name, namingNode* root, bool& inserted) {
	if (root == nullptr) {
		namingNode* n = new namingNode(name);
		inserted = true;
		return n;
	}
	else if (root->name > name) {
		root->left = insert(name, root->left, inserted);
	}
	else if (root->name < name) {
		root->right = insert(name, root->right, inserted);
	}
	else {
		cout << "Someone has used this as their userName" << endl;
	}
	return root;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool checkCityName(string str) {
	int size = str.length();
	int start = 0;
	while (start < size) {
		if (str[start] >= 'A' && str[start] <= 'Z' || str[start] >= 'a' && str[start] <= 'z') {
			start++;
			continue;
		}
		else {
			return true;
		}
		start++;
	}
	return false;
}


class users {
	node* usersList;
	namingNode* root;

public:
	users() {
		usersList = nullptr;
		root = nullptr;
	}
	bool insertName(string name) {

		bool inserted = false;
		root = insert(name, root, inserted);
		return inserted;
	}

	node* createNewUser() {
		cout << "-----------------" << endl;
		string name;
		cout << "Enter Name : ";
		getline(cin, name);
		while (!insertName(name)) {
			cout << "Try A New Name : ";
			getline(cin, name);
		}
		string password;
		cout << "(PASSWORD must be atLeast 8 character Long) .. Enter Password ... : ";
		cin >> password;
		while (password.size() <= 7) {
			cout << "Enter password Again : ";
			cin >> password;
		}
		cin.ignore();
		cin.clear();
		string city;
		cout << "Enter City : ";
		getline(cin, city);

		while (checkCityName(city)) {
			cout << "Invalid Name For a City.. Enter a Valid Name : ";
			getline(cin, city);
		}
		string currentTime = getCurrentTime();
		node* newUser = new node(name, password, city,currentTime);
		return newUser;
		cout << "-----------------" << endl;
	}

	void addNewUser() {
		node* user = createNewUser();
		if (usersList == nullptr) {
			usersList = user;
		}
		else {
			node* temp = usersList;
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = user;
		}
		/////////////////////////////////////////////
		Sleep(500);
		system("CLS");
		while (true) {
			cout << "1. Manage Profile" << endl;
			cout << "2. Manage FriendShips" << endl;
			cout << "3. Log Out" << endl;
			cout << "Enter Choice : ";
			int options = 0;
			cin >> options;
			if (options == 1) {
				system("CLS");
				selectOptions(user,usersList, root);
			}
			else if (options == 2) {
				system("CLS");
				manageFriendShips(user, usersList);
			}
			else {
				system("CLS");
				cout << "Logging Out..." << endl;
				Sleep(500);
				break;
			}

			system("CLS");
			//////////////////////////////////////
		}
	}

	void print() {
		if (usersList == nullptr) {
			cout << "NO USER EXIST..." << endl;
			return;
		}
		node* temp = usersList;
		while (temp) {
			temp->printInfo();
			temp = temp->next;
		}
	}

	void accessUser() {
		string nameForSearch;
		cout << "Enter name : ";
		getline(cin, nameForSearch);
		string searchForPassword;
		cout << "Enter password : ";
		cin >> searchForPassword;
		node* user = usersList;
		while (user) {
			if (user->name == nameForSearch && user->password == searchForPassword) {
				break;
			}
			user = user->next;
		}
		if (!user) {
			cout << "NO USER WITH SUCH CREDENTIALS EXISTS .... " << endl;
			Sleep(1000);
			return;
		}
		system("CLS");
		cout << "Welcome Back," << user->name << "." << endl;
		Sleep(500);
		while (true) {
			cout << "1. Manage Profile" << endl;
			cout << "2. Manage FriendShips" << endl;
			cout << "3. Log Out" << endl;
			cout << "Enter Choice : ";
			int options = 0;
			cin >> options;
			if (options == 1) {
				system("CLS");
				selectOptions(user,usersList,root);
			}
			else if (options == 2) {
				system("CLS");
				manageFriendShips(user, usersList);
			}
			else {
				system("CLS");
				cout << "Logging Out..." << endl;
				Sleep(500);
				break;
			}

			system("CLS");
		}
		
	}


};

void applicationMenu() {
	cout << "------------------------ INSTAGRAM --------------------------" << endl;
	cout << "1. Sign Up ............... ( CREATE NEW ACCOUNT ) " << endl;
	cout << "2. Sign In ............... ( IF YOU ALREADY HAVE ONE ) " << endl;
}

///////////////////////// updating the name /////////////////////////////////

int main()
{
	
	users usersList;
	int options = 0;
	while (true) {
		applicationMenu();
		cin >> options;
		system("CLS");
		cin.ignore();
		if (options == 1) {
			cout <<"CREATING A NEW USER.... "<<endl;
			usersList.addNewUser();
		}
		else if (options == 2) {
			cout << "ENTER CREDENTIALS...." << endl;
			usersList.accessUser();
		}
		else if (options == 3) {
			Sleep(1000);
			cout << "Exiting The Application ...." << endl;
			Sleep(1000);
			break;
		}
		else {
			if (cin.fail()) {
				cin.clear();
				cin.ignore();
			}
			
		}
		system("CLS");
	}
	
	return 0;
}