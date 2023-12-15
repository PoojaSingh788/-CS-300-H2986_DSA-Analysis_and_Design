// CourseList.cpp : Defines the entry point for the application.
//

#include "CourseList.h"
#include <vector>
#include "CSVparser.hpp"
#include <set>
#include <string>
#include <unordered_map>


using namespace std;
struct Course {
 
    std::string courseId;
    std::string name;
    std::vector<std::string> courseIds;
};

typedef std::unordered_map<size_t, shared_ptr<Course>> UMap;
typedef  shared_ptr<UMap> SpCourseMap;
/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses
 */
SpCourseMap loadCourse(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;
    SpCourseMap courseMap = make_shared <UMap>();

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    /*
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;
    */
    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            //std::string courseid = file[i][0];
            //cout << "Look for " << file[i][0] << "\n";
            size_t hash = std::hash<std::string>{}(file[i][0]);
            auto it = courseMap->find(hash);
            shared_ptr<Course > course = nullptr;

            if (it != courseMap->end()) {
                //value exist
                course = it->second;
                //cout << "Found in hash " << file[i][0] << "\n";
                //It has to be empty course as it was added as course dependency
            }
            else {
                //cout << "Not Found in hash " << file[i][0] << "\n";
                course = make_shared<Course>();
                //enter the course in map as its new course
                (*courseMap)[hash] = course;
            }
            course->courseId = file[i][0];
            course->name = file[i][1];
           
            //Iterate on course dependency and add new course node in hashmap if they do not exist
            for (int j = 2; j < file[i].size(); j++) {
                //cout << "Adding dependency " << file[i][j] << "\n";
                if (file[i][j] == "")
                    continue;
                hash = std::hash<std::string>{}(file[i][j]);
                auto it = courseMap->find(hash);
                if (it == courseMap->end()) {
                    //No course exist hash map so create and add new empty course
                    shared_ptr<Course> dependentCourse = make_shared<Course>();
                    dependentCourse->name = file[i][j];
                    (*courseMap)[hash] = dependentCourse;
                    course->courseIds.push_back(dependentCourse->name);
                    //cout << "Adding dependency in hashmap " << file[i][j] << "\n";

                }
                else {
                    course->courseIds.push_back(it->second->name);
                    //cout << "Adding dependency in hashmap " << file[i][j] << "\n";
                }
            }
            //cout << "/*****************/\n";
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }

    return courseMap;
}

/**
* Iterate on hashmap and put the names into set,
* then iterate on set and print the names of course
**/
void listCoursesInSortedOrder(SpCourseMap courseMap)
{
    auto cmp = [](const string& x, const string& y) { return x._Equal(y); };
    //set<string, bool (*)(const string&, const string&)> names(cmp);
    set<string> names;
    for (auto const& [key, val] : *courseMap)
    {
 //       std::cout << val->courseId        // string (key)
 //           << ':'
 //           << val->name      // string's value
 //           << std::endl;
        names.insert(val->name);
    }

    for (auto const& key : names)
    {
        std::cout << key     // string's value
            << std::endl;
    }
}

void printCourse(SpCourseMap courseMap, string courseId) 
{
    size_t hash = std::hash<std::string>{}(courseId);
    shared_ptr<Course> course = (*courseMap)[hash];
    if (course != nullptr) {
        cout << course->courseId << " " << course->name << "\n";
        cout << "Prerequisite Subjects are:\n";
        int i = 0;
        for (auto& x : course->courseIds) {
            cout << i << " " << x << "\n";
            i++;
        }
    }
    cout << "\n";
}
int main(int argc, char* argv[])
{
    string csvPath, course;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        course = "CSCI101";
        break;
    case 3:
        csvPath = argv[1];
        course = argv[2];
        break;
    default:
        csvPath = "C:\\Users\\pooja\\source\\repos\\CourseList\\courses.csv";
        course = "CSCI300";
    }

    SpCourseMap courseMap = nullptr;
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Course" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
        {
            courseMap = loadCourse(csvPath);
            break;
        }
        case 2:
        {
            listCoursesInSortedOrder(courseMap);
            break;
        }
        case 3:
        {
            printCourse(courseMap, course);
            break;
        }
        case 9:
            cout << "Bbye";
            return 0;
        }
    }   
    return 0;
}
