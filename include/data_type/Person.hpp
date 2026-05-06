
typedef char* person_ID;

class person {
private:
person_ID id;
char* first_name;
char* middle_name;
char* last_name;
role role;
public:
person_ID get_ID();
char* get_first_name();
char* get_middle_name();
char* get_last_name();
person();
~person();
person();
person(const char* id, const char* first, const char* middle, const char* last);
person(const person& other);
person& operator=(const person& copy);
};

enum class role{
    student,
    teacher
};