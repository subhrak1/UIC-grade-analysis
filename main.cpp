/*main.cpp*/

//
// <SUBHRA KANUNGO, SKANUN2>
// U. of Illinois, Chicago
// CS 341, Fall 2019
// Project #03: GradeUtil UI
// This program is C++11 dependent
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h> 
using namespace std;

// includes for gradeutil
#include "gradeutil.h"

College InputGradeData(string filename)
{
    College college;
    ifstream file(filename);
    string line, value;

    if (!file.good())
    {
        cout << "**Error: unable to open input file '" << filename << "'." << endl;
        return college;
    }

    // first line contains semester,year
    getline(file, line);
    stringstream ss(line);

    getline(ss, college.Name, ',');
    getline(ss, college.Semester, ',');
    getline(ss, value);
    college.Year = stoi(value);

    // second line contains column headers --- skip
    getline(file, line);

    //
    // now start inputting and parse course data:
    //

    while (getline(file, line))
    {
        Course c = ParseCourse(line);

        //
        // search for correct dept to ask course to, otherwise create a new dept:
        //
        auto dept_iter = std::find_if(college.Depts.begin(),
                                      college.Depts.end(),
                                      [&](const Dept &d) {
                                          return (d.Name == c.Dept);
                                      });

        if (dept_iter == college.Depts.end())
        {
            //
            // doesn't exist, so we have to create a new dept
            // and insert course:
            //
            Dept d(c.Dept);

            d.Courses.push_back(c);

            college.Depts.push_back(d);
        }
        else
        {
            // dept exists, so insert course into existing dept:
            dept_iter->Courses.push_back(c);
        }

    } //while

    //
    // done:
    //
    return college;
}

void printGradeDistribution(College& college){
  
    GradeStats scoreToPrint = GetGradeDistribution(college);
    
    cout<< "grade distribution (A-F): " << scoreToPrint.PercentA << "%, " <<scoreToPrint.PercentB << "%, " <<scoreToPrint.PercentC << "%, " <<scoreToPrint.PercentD << "%, " <<scoreToPrint.PercentF << "% " << endl;
  
}

void printDFWrate(College& college){
    
    int DFW = 0;
    int N = 0;
    double DFWrate = GetDFWRate(college, DFW, N);
    cout<< "DFW rate: " << DFWrate<< "% " << endl;     
}
void printGradeDistribution(Dept& d){
  
    GradeStats scoreToPrint = GetGradeDistribution(d);
    
    cout<< " grade distribution (A-F): " << scoreToPrint.PercentA << "%, " <<scoreToPrint.PercentB << "%, " <<scoreToPrint.PercentC << "%, " <<scoreToPrint.PercentD << "%, " <<scoreToPrint.PercentF << "% " << endl;
  
}

void printDFWrate(Dept& d){
    
    int DFW = 0;
    int N = 0;
    double DFWrate = GetDFWRate(d, DFW, N);
    cout<< " DFW rate: " << DFWrate<< "% " << endl;     
}
void printGradeDistribution(Course& c){
  
    GradeStats scoreToPrint = GetGradeDistribution(c);
    
    cout<< " grade distribution (A-F): " << scoreToPrint.PercentA << "%, " <<scoreToPrint.PercentB << "%, " <<scoreToPrint.PercentC << "%, " <<scoreToPrint.PercentD << "%, " <<scoreToPrint.PercentF << "% " << endl;
  
}

void printDFWrate(Course& c){
    
    int DFW = 0;
    int N = 0;
    double DFWrate = GetDFWRate(c, DFW, N);
    cout<< " DFW rate: " << DFWrate<< "% " << endl;     
}
void enumConversion(Course& c){
  
  if(c.getGradingType() == 0){
    cout<< " course type: " << "letter" << endl;
  }
  else if(c.getGradingType() == 1){
    cout<< " course type: " << "satisfactory" << endl;
  }
  else{
    cout<< " course type: " << "unknown" << endl;
  }  
}
void printCollegeSummary(College& college){
     
    cout<< "** College of " << college.Name << ", " << college.Semester << " " << college.Year << " **" << endl;
    cout<< "# of courses taught: " << college.NumCourses() << endl;
    cout<< "# of students taught: " << college.NumStudents() << endl;
    printGradeDistribution(college);
    printDFWrate(college);
    cout<< "\n" << "Enter a command> ";
    
}
void printSummary(College& college){
  cout<<"dept name, or all? ";      
  string input;
  cin >> input;
  
  if(input == "all"){
     vector<Dept> dept = college.Depts;
     sort(dept.begin(), dept.end(),
          [](const Dept& a, const Dept& b){
            return a.Name < b.Name;
          });
      for(auto &dep: dept){
          cout<< dep.Name <<": " << endl;
          cout<< " # courses taught: " << dep.NumCourses() << endl;
          cout<< " # students taught: " << dep.NumStudents() << endl;
          printGradeDistribution(dep);
          printDFWrate(dep);
      }
  }
  else{
    auto dept_iter = find_if(college.Depts.begin(), college.Depts.end(),
                           [&](const Dept& dept_iter){
                             if(dept_iter.Name == input)
                               return true; 
                             else
                               return false;
                           });
                           
    if(dept_iter != college.Depts.end()){
       cout<< dept_iter->Name <<": " << endl;
       cout<< " # courses taught: " << dept_iter->NumCourses() << endl;
       cout<< " # students taught: " << dept_iter->NumStudents() << endl;
       Dept d = *dept_iter;
       printGradeDistribution(d);
       printDFWrate(d);
    }
    else{
      cout << "**dept not found" << endl;
      return;
    }
  }
}
void printSearch(College& college){
  string instructorPrefix, dept;
  int courseNum;
  string enumConvert;
  
  cout<<"dept name, or all? ";
  cin>>dept;
  cout<<"course # or instructor prefix? ";
  cin>>instructorPrefix;
  
  stringstream ss(instructorPrefix);
  ss >> courseNum;
   
  if(dept == "all"){
    if(ss.fail()){
      vector<Course> courses = FindCourses(college, instructorPrefix);     
      auto cour_iter = find_if(courses.begin(), courses.end(),
                               [&](const Course &cour_iter)
                               {
                                 std::string prefix(instructorPrefix);
                                 std::string prefixCheck(cour_iter.Instructor);
      
                                 auto res = std::mismatch(prefix.begin(), prefix.end(), prefixCheck.begin());
                                 if (res.first == prefix.end()){
                                   return true;
                                 }
                                 else{
                                   return false;
                                 }
                               });
      if(cour_iter == courses.end()){
        cout<<"**none found"<<endl;
        return;
      }
      while(cour_iter != courses.end()){
          Course c = *cour_iter;
          cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
          cout<< " # students: " << c.getNumStudents() << endl;
          enumConversion(c);
          printGradeDistribution(c);
          printDFWrate(c);
        
          cour_iter = find_if(++cour_iter, courses.end(),
                               [&](const Course &cour_iter)
                               {
                                 std::string prefix(instructorPrefix);
                                 std::string prefixCheck(cour_iter.Instructor);
      
                                 auto res = std::mismatch(prefix.begin(), prefix.end(), prefixCheck.begin());
                                 if (res.first == prefix.end()){
                                   return true;
                                 }
                                 else{
                                   return false;
                                 }
                               });        
      }
  
    }
    else{
      vector<Course> courses = FindCourses(college, courseNum);
      auto iterNum = find_if(courses.begin(), courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Number == courseNum)
                                   return true;
                                 else
                                   return false;
                               });
      if(iterNum == courses.end()){
        cout << "**none found"<< endl;
        return;
      }
      while(iterNum != courses.end()){
          cout<< iterNum->Dept << " " << iterNum->Number << " " << "(section " << iterNum->Section << "): " << iterNum->Instructor << endl;
          cout<< " # students: " << iterNum->getNumStudents() << endl;
          Course c = *iterNum;
          enumConversion(c);
          printGradeDistribution(c);
          printDFWrate(c);
        
          iterNum = find_if(++iterNum, courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Number == courseNum)
                                   return true;
                                 else
                                   return false;
                               });    
      }     
    }
  }
  else{
     auto deptit = find_if(college.Depts.begin(), college.Depts.end(),
                             [&](const Dept& deptit){
                                 if(deptit.Name == dept)
                                   return true; 
                                 else
                                   return false;
                               });
    
    if(deptit == college.Depts.end()){
      cout <<"**dept not found"<<endl;
      return;
    }
    else{
       Dept d = *deptit;
       if(ss.fail()){
              vector<Course> courses = FindCourses(d, instructorPrefix);
              auto cour_iter = find_if(courses.begin(), courses.end(),
                               [&](const Course &cour_iter)
                               {
                                 std::string prefix(instructorPrefix);
                                 std::string prefixCheck(cour_iter.Instructor);
      
                                 auto res = std::mismatch(prefix.begin(), prefix.end(), prefixCheck.begin());
                                 if (res.first == prefix.end()){
                                   return true;
                                 }
                                 else{
                                   return false;
                                 }
                               });
              if(cour_iter == courses.end()){
                  cout<<"**none found"<<endl;
                  return;
              }
              while(cour_iter != courses.end()){
                    Course c = *cour_iter;
                    cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
                    cout<< " # students: " << c.getNumStudents() << endl;
                    enumConversion(c);
                    printGradeDistribution(c);
                    printDFWrate(c);
        
                    cour_iter = find_if(++cour_iter, courses.end(),
                               [&](const Course &cour_iter)
                               {
                                 std::string prefix(instructorPrefix);
                                 std::string prefixCheck(cour_iter.Instructor);
      
                                 auto res = std::mismatch(prefix.begin(), prefix.end(), prefixCheck.begin());
                                 if (res.first == prefix.end()){
                                   return true;
                                 }
                                 else{
                                   return false;
                                 }
                               });        
                      }    
             }
        else{
              vector<Course> courses = FindCourses(d, courseNum);
              auto iterNum = find_if(courses.begin(), courses.end(),
                                   [&](const Course& iterNum)
                               {
                                 if(iterNum.Number == courseNum)
                                   return true;
                                 else
                                   return false;
                               });
              if(iterNum == courses.end()){
                 cout << "**none found"<< endl;
                 return;
              }
              while(iterNum != courses.end()){
                    cout<< iterNum->Dept << " " << iterNum->Number << " " << "(section " << iterNum->Section << "): " << iterNum->Instructor << endl;
                    cout<< " # students: " << iterNum->getNumStudents() << endl;
                    Course c = *iterNum;
                    enumConversion(c);
                    printGradeDistribution(c);
                    printDFWrate(c);
        
                    iterNum = find_if(++iterNum, courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Number == courseNum)
                                   return true;
                                 else
                                   return false;
                               });    
               }
           }
    }
    /*while(deptit != college.Depts.end()){
       Dept d = *deptit;
       if(ss.fail()){
              vector<Course> courses = FindCourses(d, instructorPrefix);
              for(auto &c: courses){
                  cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
                  cout<< " # students: " << c.getNumStudents() << endl;
                  enumConversion(c);
                  printGradeDistribution(c);
                  printDFWrate(c);    
              }    
            }
           else{
              vector<Course> courses = FindCourses(d, courseNum);
              for(auto &c: courses){
                  cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
                  cout<< " # students: " << c.getNumStudents() << endl;            
                  enumConversion(c);
                  printGradeDistribution(c);
                  printDFWrate(c);    
              }    
           }
        }*/
    }
    /*for(auto &d: college.Depts){     
        if(d.Name == dept){
           if(ss.fail()){
              vector<Course> courses = FindCourses(d, instructorPrefix);
              for(auto &c: courses){
                  cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
                  cout<< " # students: " << c.getNumStudents() << endl;
                  enumConversion(c);
                  printGradeDistribution(c);
                  printDFWrate(c);    
              }    
            }
           else{
              vector<Course> courses = FindCourses(d, courseNum);
              for(auto &c: courses){
                  cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
                  cout<< " # students: " << c.getNumStudents() << endl;            
                  enumConversion(c);
                  printGradeDistribution(c);
                  printDFWrate(c);    
              }    
           }    
         }
      }*/
   }  

void printSatisfactory(College& college){
  string input;
  
  cout<<"dept name, or all? ";
  cin>>input;
  vector<Course> courses;
  
  for(auto &d: college.Depts){
      for(auto &c: d.Courses){
          if(c.getGradingType() == 1){
              courses.push_back(c);
          }
      }
  }
  sort(courses.begin(), courses.end(), 
           [](const Course& a, const Course& b)
      {
          if (a.Dept == b.Dept){
              if(a.Number == b.Number){
                    return a.Section < b.Section;   
              }
              else{
                  return a.Number < b.Number;
              }
          }
          return a.Dept < b.Dept;
      });
    
  if(input == "all"){
    for(auto &c1: courses){
           cout<< c1.Dept << " " << c1.Number << " " << "(section " << c1.Section << "): " << c1.Instructor << endl;
           cout<< " # students: " << c1.getNumStudents() << endl; 
           cout<< " course type: " << "satisfactory" << endl;
      }
    }
  else{
      auto iterNum = find_if(courses.begin(), courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Dept == input)
                                   return true;
                                 else
                                   return false;
                               });
    
      if(iterNum == courses.end()){
        cout << "**none found"<< endl;
        return;
      }
      while(iterNum != courses.end()){
          Course c1 = *iterNum;
          cout<< c1.Dept << " " << c1.Number << " " << "(section " << c1.Section << "): " << c1.Instructor << endl;
          cout<< " # students: " << c1.getNumStudents() << endl; 
          cout<< " course type: " << "satisfactory" << endl; 
        
          iterNum = find_if(++iterNum, courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Dept == input)
                                   return true;
                                 else
                                   return false;
                               });    
      }     
    }
  /*for(auto &c1: courses){
      if(input == "all"){
           cout<< c1.Dept << " " << c1.Number << " " << "(section " << c1.Section << "): " << c1.Instructor << endl;
           cout<< " # students: " << c1.getNumStudents() << endl; 
           cout<< " course type: " << "satisfactory" << endl;        
      }
      else{
         if(c1.Dept == input){
               cout<< c1.Dept << " " << c1.Number << " " << "(section " << c1.Section << "): " << c1.Instructor << endl;
               cout<< " # students: " << c1.getNumStudents() << endl; 
               cout<< " course type: " << "satisfactory" << endl;   
          }
       } 
    }*/
}
void printDFW(College& college){
  string input;
  cout<<"dept name, or all? ";
  cin>>input;
  
  vector<Course> courses;
  double dfwThreshold;
  double dfwRate;
  int DFW = 0;
  int N = 0;
  
  cout<<"dfw threshold? ";
  cin>> dfwThreshold;
  
  if(input == "all"){
     for(auto &d: college.Depts){
        for(auto &c: d.Courses){
          dfwRate = GetDFWRate(c, DFW, N);
          if(dfwRate > dfwThreshold){
          courses.push_back(c);
        }
       }
     }
  }
  else{
    auto iterNum = find_if(college.Depts.begin(), college.Depts.end(),
                               [&](const Dept& iterNum)
                               {
                                 if(iterNum.Name == input)
                                   return true;
                                 else
                                   return false;
                               });
    
    if(iterNum != college.Depts.end()){
      for(auto &c : iterNum->Courses){
        dfwRate = GetDFWRate(c, DFW, N);
        if(dfwRate > dfwThreshold){
          courses.push_back(c);
        }
      }
    }
    else{
        cout << "**dept not found"<< endl;
        return;
      }
  }
  if (courses.size() == 0){
    cout <<"**none found"<<endl;
  }
  
  sort(courses.begin(), courses.end(),
          [](const Course& a, const Course& b)
       {
         int DFW = 0;
         int N = 0;
         double dfwRateA = GetDFWRate(a, DFW, N);
         double dfwRateB = GetDFWRate(b, DFW, N);
         
         if(dfwRateA != dfwRateB){
           return dfwRateA > dfwRateB;
         }
         else{
            if(a.Dept == b.Dept){
               if(a.Number == b.Number){
                  return a.Section < b.Section;
               }
               else{
                  return a.Number < b.Number;             
              }
            }
           return a.Dept < b.Dept;
         }
      });
    
   for(auto &c : courses){
        cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
        cout<< " # students: " << c.getNumStudents() << endl;
        cout<< " course type: " <<"letter" << endl;
        printGradeDistribution(c);
        printDFWrate(c);
   }
   
  
 /* for(auto &d: college.Depts){
    for(auto &c: d.Courses){
      dfwRate = GetDFWRate(c, DFW, N);
        if(dfwRate > dfwThreshold){
          courses.push_back(c);
      }
    }
  }
  sort(courses.begin(), courses.end(),
          [](const Course& a, const Course& b)
       {
         int DFW = 0;
         int N = 0;
         double dfwRateA = GetDFWRate(a, DFW, N);
         double dfwRateB = GetDFWRate(b, DFW, N);
         
         if(dfwRateA != dfwRateB){
           return dfwRateA > dfwRateB;
         }
         else{
            if(a.Dept == b.Dept){
               if(a.Number == b.Number){
                  return a.Section < b.Section;
               }
               else{
                  return a.Number < b.Number;             
              }
            }
           return a.Dept < b.Dept;
         }
      });
  
  if(input == "all"){
    if(courses.size()==0){
      cout<<"**none found"<<endl;
    }
    else{
      for(auto &c: courses){
        cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
        cout<< " # students: " << c.getNumStudents() << endl;
        cout<< " course type: " <<"letter" << endl;
        printGradeDistribution(c);
        printDFWrate(c);
      } 
    }
  }
  else {  
    auto iterNum = find_if(courses.begin(), courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Dept == input)
                                   return true;
                                 else
                                   return false;
                               });
    
      if(iterNum == courses.end()){
        cout << "**dept not found"<< endl;
        return;
      }
      else{
        while(iterNum != courses.end()){
          Course c = *iterNum;
          cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
          cout<< " # students: " << c.getNumStudents() << endl; 
          cout<< " course type: " << "letter" << endl;
          printGradeDistribution(c);
          printDFWrate(c);
        
          iterNum = find_if(++iterNum, courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Dept == input)
                                   return true;
                                 else
                                   return false;
                               });    
           }  
      }         
  } */

}

void printLetterB(College& college){
    string input;
    cout<<"dept name, or all? ";
    cin>>input;
  
    vector<Course> courses;
    double letterBThresh;
    GradeStats scoreToCheck;

  
    cout<<"letter B threshold? ";
    cin>> letterBThresh;
      
  if(input == "all"){
     for(auto &d: college.Depts){
        for(auto &c: d.Courses){
          scoreToCheck = GetGradeDistribution(c);
          if(scoreToCheck.PercentB > letterBThresh){
             courses.push_back(c);
          }
        }
       }
     }
  else{
    auto iterNum = find_if(college.Depts.begin(), college.Depts.end(),
                               [&](const Dept& iterNum)
                               {
                                 if(iterNum.Name == input)
                                   return true;
                                 else
                                   return false;
                               });
    
    if(iterNum != college.Depts.end()){
      for(auto &c : iterNum->Courses){
          scoreToCheck = GetGradeDistribution(c);
          if(scoreToCheck.PercentB > letterBThresh){
             courses.push_back(c);
          }
      }
    }
    else{
        cout << "**dept not found"<< endl;
        return;
      }
  }
  if (courses.size() == 0){
    cout <<"**none found"<<endl;
  }
  
  sort(courses.begin(), courses.end(),
          [](const Course& a, const Course& b)
       {
         GradeStats scoreToCheckA = GetGradeDistribution(a);
         GradeStats scoreToCheckB = GetGradeDistribution(b);
         
         if(scoreToCheckA.PercentA != scoreToCheckB.PercentB){
           return scoreToCheckA.PercentB > scoreToCheckB.PercentB;
         }
         else{
            if(a.Dept == b.Dept){
               if(a.Number == b.Number){
                  return a.Section < b.Section;
               }
               else{
                  return a.Number < b.Number;             
              }
            }
           return a.Dept < b.Dept;
         }
      });
    
   for(auto &c : courses){
      cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
      cout<< " # students: " << c.getNumStudents() << endl;
      cout<< " course type: " <<"letter" << endl;
      printGradeDistribution(c);
      printDFWrate(c);
   }
   
  /*for(auto &d: college.Depts){
      for(auto &c: d.Courses){
          scoreToCheck = GetGradeDistribution(c);
          if(scoreToCheck.PercentB > letterBThresh){
             courses.push_back(c);
          }
        }
     }
  sort(courses.begin(), courses.end(),
          [](const Course& a, const Course& b)
       {
         GradeStats scoreToCheckA = GetGradeDistribution(a);
         GradeStats scoreToCheckB = GetGradeDistribution(b);
         
         if(scoreToCheckA.PercentA != scoreToCheckB.PercentB){
           return scoreToCheckA.PercentB > scoreToCheckB.PercentB;
         }
         else{
            if(a.Dept == b.Dept){
               if(a.Number == b.Number){
                  return a.Section < b.Section;
               }
               else{
                  return a.Number < b.Number;             
              }
            }
           return a.Dept < b.Dept;
         }
      });
  
  if(input == "all"){
    if(courses.size()==0){
      cout<<"**none found"<<endl;
    }
    else{
      for(auto &c: courses){
      cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
      cout<< " # students: " << c.getNumStudents() << endl;
      cout<< " course type: " <<"letter" << endl;
      printGradeDistribution(c);
      printDFWrate(c);
      }
    }
  }
  else { 
      auto iterNum = find_if(courses.begin(), courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Dept == input)
                                   return true;
                                 else
                                   return false;
                               });
    
      if(iterNum == courses.end()){
        cout << "**none found"<< endl;
        return;
      }
      while(iterNum != courses.end()){
          Course c = *iterNum;
          cout<< c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
          cout<< " # students: " << c.getNumStudents() << endl; 
          cout<< " course type: " << "letter" << endl;
          printGradeDistribution(c);
          printDFWrate(c);
        
          iterNum = find_if(++iterNum, courses.end(),
                               [&](const Course& iterNum)
                               {
                                 if(iterNum.Dept == input)
                                   return true;
                                 else
                                   return false;
                               });    
      }     
    } */ 
}
double calculateGPA(Dept d){
  
  int counter =0;
  double avg;
  for(Course c: d.Courses){
    GradeStats stats;
    stats = GetGradeDistribution(c);
    if(c.getGradingType() == 0){
      counter++;
      double gpa = stats.NumA * 4.0 + stats.NumB * 3.0 + stats.NumC * 2.0 + stats.NumD * 1.0;
      double num = stats.NumA + stats.NumB + stats.NumC + stats.NumD + stats.NumF;
       avg += double(gpa/num);
    }
  
  }
  
  
  double gpa = avg/counter;
  return gpa;
}

void sortGPA(vector <Dept> &dept){
    sort(dept.begin(), dept.end(),
          [](const Dept& a, const Dept& b)
       {
         
            if(calculateGPA(a) == calculateGPA(b)){
               
            }
           return calculateGPA(a) > calculateGPA(b);
         
      });
}


void writeGPA(vector <Dept> dept){
  for(Dept d: dept){
     cout<<"Overall GPA for "<<d.Name<<" : "<<calculateGPA(d)<<endl;
  }
}

double calculateGPACourse(Course c){
  GradeStats stats;
  stats = GetGradeDistribution(c);
  double gpa = stats.NumA * 4.0 + stats.NumB * 3.0 + stats.NumC * 2.0 + stats.NumD * 1.0;
      double num = stats.NumA + stats.NumB + stats.NumC + stats.NumD + stats.NumF;
      double avg = double(gpa/num);
  return avg;
}

void sortCourseGPA(vector <Course> &courses){
  sort(courses.begin(), courses.end(),
          [](const Course& a, const Course& b)
       {
         
            if(calculateGPACourse(a) == calculateGPACourse(b)){
               if(a.Number == b.Number){
                 return a.Section < b.Section;
               }
              return a.Number < b.Number;
            }
           return calculateGPACourse(a) > calculateGPACourse(b);         
      });  
}

void printAverage(College& college){
    string input;
    cout<<"dept name, or all? ";
    cin>>input;
  vector<Course> courses;
  vector<Dept> dept;
    
    if(input == "all"){
      for(Dept d: college.Depts){
        GradeStats stats = GetGradeDistribution(d);
        if(stats.N != 0){
          dept.push_back(d);
        }
      }      
      sortGPA(dept);
      writeGPA(dept);
    }
  else{
    //dept name has been given
    for(Dept d: college.Depts){
      if(d.Name == input){
        for(Course c: d.Courses){
          if(c.getGradingType() == Course::Letter){
            courses.push_back(c);
          }           
         }
      }
    }
    sortCourseGPA(courses);
    for(Course c: courses){
      cout<<"Overall GPA for "<<c.Dept<< " " << c.Number << "(" <<c.Section<< ") :"<<" "<<calculateGPACourse(c)<<endl;
    }
  }
}


// TODO: define your own functions
//
//

int main()
{
    string filename;

    cout << std::fixed;
    cout << std::setprecision(2);

    //
    // 1. Input the filename and then the grade data:
    //
    cin >> filename;
    //filename = "fall-2018.csv";

    College college = InputGradeData(filename);
    
    printCollegeSummary(college);
   /* cout<< "** College of Engineering, Fall 2018 **" << endl;
    cout<< "# of courses taught: " << college.NumCourses() << endl;
    cout<< "# of students taught: " << college.NumStudents() << endl;
    printGradeDistribution(college);
    printDFWrate(college);
    cout<< "\n" << "Enter a command> ";*/
  
    string command;
    cin >> command;
  
    while(command != "#"){
      if(command == "summary"){  
          printSummary(college);
        }
      else if(command == "search"){
          printSearch(college);
        }
      else if(command == "dfw"){
          printDFW(college);
        }
      else if(command == "satisfactory"){
          printSatisfactory(college);
        }
      else if(command == "letterB"){
          printLetterB(college);
        }
      else if(command == "average"){
          printAverage(college);
        }
      else{
          cout << "**unknown command" << endl;
      }
      cout<< "Enter a command> ";
      cin >> command;
    }
    
    

    // 2. TODO: print out summary of the college
    // DEFINE your own functions

    //
    // 3. TODO: Start executing commands from the user:
    // DEFINE your own functions
    

    //
    // done:
    //
    return 0;
}