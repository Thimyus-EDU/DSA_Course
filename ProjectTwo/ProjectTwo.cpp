#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

//Struct to hold course data
struct Course {
	std::string courseNumber;
	std::string courseTitle;
	std::vector<std::string> prerequisites;
};

//Function to read from file
std::vector<Course> readFromFile(std::string filename) {
	std::ifstream inFile(filename);
	std::vector<Course> courses;


	//Checks if file is opened and returns error message if not
	if (!inFile.is_open()) {
		std::cerr << "Can't open file." << std::endl;
		return courses;
	}

	//Parses file 
	std::string line; //Holds each line of the file as its read
	//Read the file line by line
	while (std::getline(inFile, line)) {
		std::stringstream ss(line); //Processes each line as a string of text
		std::string field; //Temporary data storage
		Course course; //Temporary course storage

		//Extract the course number
		//Extracts text from the line up to the comma 
		//Stores text as course number
		if (!std::getline(ss, course.courseNumber, ',')) continue;

		//Extract the course title
		//Extracts text from the line up to the comma 
		//Stores text as course title
		if (!std::getline(ss, course.courseTitle, ',')) continue;

		//Gets any prerequisites
		while (std::getline(ss, field, ',')) {
			course.prerequisites.push_back(field);
		}

		//Add course struct to vector of courses
		courses.push_back(course);

	}

	//Close the file and return the vector
	inFile.close();
	return courses;
};

//Structure for each Node in the tree
struct Node {
	Course course;
	Node* left;
	Node* right;

	// Default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//Constructor to initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

//Class for Binary Search Tree
class BinarySearchTree {

private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	Node* removeNode(Node* node, std::string courseNumber);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course searchCourses(std::string courseNumber);
	void Remove(std::string courseNumber);
	void printAll();
};

//Default constructor for tree
BinarySearchTree::BinarySearchTree() {
	root = nullptr;

}

//Destructor for tree
BinarySearchTree::~BinarySearchTree() {
	while (root != nullptr) {
		Remove(root->course.courseNumber);
	}
}

// Function to remove a node from the tree
void BinarySearchTree::Remove(std::string courseNumber) {
	this->removeNode(root, courseNumber);

}

Node* BinarySearchTree::removeNode(Node* node, std::string courseNumber) {
	if (node == nullptr) {
		return node;
	}

	if (node->course.courseNumber > courseNumber) {
		node->left = removeNode(node->left, courseNumber);
	}
	else if (node->course.courseNumber < courseNumber) {
		node->right = removeNode(node->right, courseNumber);
	}
	else {
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		else if (node->left != nullptr && node->right == nullptr) {
			Node* temp = node;
			node = node->left;
			delete temp;
		}
		else if (node->left == nullptr && node->right != nullptr) {
			Node* temp = node;
			node = node->right;
			delete temp;
		}
		else {
			Node* temp = node->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			node->course = temp->course;
			node->right = removeNode(node->right, temp->course.courseNumber);
		}
	}
	return node;

}

//Traverse the tree in order
void BinarySearchTree::InOrder() {
	inOrder(root);
}

//Function to insert a course into the tree
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}

}

//Function to load data from course vector into tree
void loadData(BinarySearchTree& tree, std::string filename) {
	std::vector<Course> courses = readFromFile(filename);
	for (const auto& course : courses) {
		tree.Insert(course);
	}
}

//Function to traverse tree in order printing each course
void BinarySearchTree::printAll() {
	InOrder();
}

//Function to search tree to find specific course
Course BinarySearchTree::searchCourses(std::string courseNumber) {
	Node* currentNode = root;

	while (currentNode != nullptr) {
		if (currentNode->course.courseNumber == courseNumber) {
			return currentNode->course;
		}
		if (currentNode->course.courseNumber > courseNumber) {
			currentNode = currentNode->left;
		}
		else {
			currentNode = currentNode->right;
		}
	}
	Course course;
	return course;
}

//Function to add a course to the tree
void BinarySearchTree::addNode(Node* node, Course course) {

	if (node->course.courseNumber > course.courseNumber) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}

	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}

}

//Function to traverse tree in order
void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		std::cout << node->course.courseNumber << ": " << node->course.courseTitle << std::endl;
		inOrder(node->right);
	}

}


int main() {
	int userInput;
	BinarySearchTree tree;
	std::string courseNum;
	std::string filename;

	std::cout << "Enter the name of the input file that holds the course data: " << std::endl;
	std::cin >> filename;

	//Do while loop to display menu until user exits
	do {
		//Display menu options
		std::cout << "\nChoose an option:\n"
			<< "1. Load Data\n"
			<< "2. Print All Courses\n"
			<< "3. Search for a Course\n"
			<< "9. Exit\n"
			<< "Enter your choice: ";
		std::cin >> userInput;

		// Input validation
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Please enter a number.\n";
			continue;
		}

		//Switch case statement to menu choices
		switch (userInput) {
		case 1:
			loadData(tree, filename);
			break;
		case 2:
			tree.printAll();
			break;
		case 3: {
			std::cout << "Enter course number: ";
			std::cin >> courseNum;

			Course result = tree.searchCourses(courseNum);

			if (!result.courseNumber.empty()) {
				std::cout << result.courseNumber << ": " << result.courseTitle;

				if (!result.prerequisites.empty()) {
					std::cout << " | Prerequisites: ";
					for (const auto& prereq : result.prerequisites) {
						std::cout << prereq << " ";
					}
				}
				std::cout << std::endl;
			}
			else {
				std::cout << "Course not found." << std::endl;
			}

			break;
		}
		case 9:
			break;

		default:
			std::cout << "invalid input" << std::endl;
		}
	} while (userInput != 9);

	return 0;
}

