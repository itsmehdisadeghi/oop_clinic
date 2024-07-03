#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
using namespace std;


bool find(int id)
{
    bool found = false;
    ifstream patient_db("db_patient.txt");
    string line;
    while (getline(patient_db, line))
    {
        string patientId = line.substr(0, line.find('|'));
        int patientIdInt = stoi(patientId);
        if (patientIdInt == id)
        {
            patient_db.close();
            found = true;
        }
    }
    ifstream doctor_db("db_doctor.txt");
    while (getline(doctor_db, line))
    {
        string doctorId = line.substr(0, line.find('|'));
        int doctorIdInt = stoi(doctorId);
        if (doctorIdInt == id)
        {
            doctor_db.close();
            found = true;
        }
    }
    return found;
}

class c_date
{
private:
    int _day,_month,_year;

    chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    struct tm* localTime = localtime(&now_c);

    int today_year() {return localTime->tm_year + 1900;}
    int today_month() {return localTime->tm_mon + 1;}
    int today_day() {return localTime->tm_mday;}
public:
    void get_date()
    {
        cout << " day : ";
        cin >> _day;
        cout << " month : ";
        cin >> _month;
        cout << " year : ";
        cin >> _year;
    }
    string today() { return to_string(localTime->tm_year + 1900) + '|' + to_string(localTime->tm_mon + 1) + '|' + to_string(localTime->tm_mday);}
    string date(){return to_string(_year) + "|" + to_string(_month) + "|" + to_string(_day);}
    int age(){return today_year()-_year;}
};

class c_person
{
protected:
    string  _first_name,_last_name,_birth_day,_create_date , _phone_number;
    int  _age,_national_code;
public:

    void new_person()
    {
        _nat:
        cout << " national code  : ";
        cin >> _national_code;
        if (find(_national_code))
        {
            cout << "this id is already token" << endl;
            goto _nat;
        }
        cout << " first name : ";
        cin >> _first_name;
        cout << " last name : ";
        cin >> _last_name;
        cout << " phone number : ";
        cin >> _phone_number;
        cout << " birth date : " << endl;
        c_date date1;
        date1.get_date();
        _age = date1.age();
        _birth_day = date1.date();
        cout << " create date : " << date1.today() <<endl;
        _create_date = date1.today();
    }
};

class c_doctor : public c_person
{
private:
    string  _expertise, _medical_id;
public:
    void new_doctor()
    {
        c_person::new_person();
        cout << " expertise : ";
        cin >> _expertise;
        cout << " medical id : ";
        cin >> _medical_id;
    }
    string get_data(){return to_string(_national_code) + '|' + _first_name + '|' + _last_name + '|' + _expertise + '|' + _medical_id + '|' + _phone_number + '|' + _birth_day + '|' + _create_date + '|' + to_string(_age);}
};

class c_patient : public c_person
{
private:
    string _father_name , _mother_name;
public:
    void new_patient()
    {
        c_person::new_person();
        cout << " father name : ";
        cin >> _father_name;
        cout << " mother name : ";
        cin >> _mother_name;
    }
    string get_data(){return to_string(_national_code) + '|' + _first_name + '|' + _last_name + '|' + _father_name + '|' + _father_name + '|' + _phone_number + '|' + _birth_day + '|' + _create_date + '|' + to_string(_age);}
};

class c_file
{
private:
    string db_doctor = "db_doctor.txt",db_patient = "db_patient.txt" , db_history = "db_history.txt";
public:
    void new_doctor()
    {
        c_doctor doctor;
        doctor.new_doctor();
        fstream doctors_db(db_doctor,ios::app);
        doctors_db << doctor.get_data() << endl;
        doctors_db.close();cout << "New doctor saved." << endl;
    }
    void new_patient()
    {
        c_patient patient;
        patient.new_patient();
        fstream patient_db(db_patient, ios::app);
        patient_db << patient.get_data() << endl;
        patient_db.close();cout << "New patient saved." << endl;
    }
    bool find_patient(int id)
    {
        ifstream patient_db(db_patient);
        string line;
        bool found = false;
        while (getline(patient_db, line)){
            string patientId = line.substr(0, line.find('|'));
            int patientIdInt = stoi(patientId);
            if (patientIdInt == id){patient_db.close();return true;}}
        return false;
    }
    bool find_doctor(int id)
    {
        ifstream doctor_db(db_doctor);
        string line;
        while (getline(doctor_db, line)){
            string doctorId = line.substr(0, line.find('|'));
            int doctorIdInt = stoi(doctorId);
            if (doctorIdInt == id){doctor_db.close();return true;}}
        return false;
    }
    void delete_doctor()
    {
        int id;
        cout << " doctor id : " ;
        cin >> id;
        if (find_doctor(id))
        {
            ifstream doctor_db(db_doctor);
            vector<string> temp;
            string line;
            while (getline(doctor_db, line)){
                string str_doctor_id = line.substr(0, line.find('|'));
                int doctor_id = stoi(str_doctor_id);
                if (doctor_id != id){temp.push_back(line);}}
            doctor_db.close();
            ofstream output_file(db_doctor);
            for (const string &data : temp){output_file << data << endl;}
            output_file.close();
            cout << "Doctor with id " << id << " deleted." << endl;
        }
        else{cout << "doctor with id " << id << " doesn't exist"<<endl;}
    }
    void delete_patient()
    {
        int id;
        cout << " doctor id : " ;
        cin >> id;
        if (find_patient(id))
        {
            ifstream patient_db(db_patient);
            vector<string> temp;
            string line;
            while (getline(patient_db, line)) {
                string patientId = line.substr(0, line.find('|'));
                int patientIdInt = stoi(patientId);
                if (patientIdInt != id)temp.push_back(line);
            }
            patient_db.close();
            ofstream output_file(db_patient);
            for (const string &data: temp)output_file << data << endl;
            output_file.close();
            cout << "Patient with id " << id << " deleted." << endl;
        }
        else{cout << "patient with id " << id << " doesn't exist"<<endl;}
    }
    void visit()
    {
        int pat_id , doc_id;
        cout<< " patient id : ";cin>> pat_id;
        cout<< " doctor id : ";cin>> doc_id;
        if (find_patient(pat_id)&& find_doctor(doc_id))
        {
            cin.ignore();
            c_date date;
            string prescriptions = to_string(pat_id) + '|' + to_string(doc_id) + '|' + date.today();
            string t;
            for (int i = 0; i < 5; i++)
            {
                cout << " enter the " << i + 1 << " drug : ";
                getline(cin, t);
                prescriptions += '|' + t;
            }
            cout << " enter a description : ";
            getline(cin, t);
            prescriptions += '|' + t;
            fstream history_db(db_history,ios::app);
            history_db << prescriptions << endl;
            history_db.close();
            cout << "visit saved " << endl;
        }
        else
        {
            cout << " doctor or patient doesn't exist!."<<endl;
        }
    }
    void print_doctor()
    {
        ifstream doctors_db(db_doctor);
        string line;
        while (getline(doctors_db, line))cout << line << endl;
        doctors_db.close();
    }
    void print_patient()
    {
        ifstream patient_db(db_patient);
        string line;
        while (getline(patient_db, line))
        {
            cout << line << endl;
        }
        patient_db.close();
    }
    void print_history()
    {
        ifstream history_db(db_history);
        string line;
        while (getline(history_db, line))
        {
            cout << line << endl;
        }
        history_db.close();
    }
};

void menu()
{
    cout << "----------------------" << endl;
    cout << " 1   DOCTOR SERVICES" << endl;
    cout << " 2   PATIENT SERVICES" << endl;
    cout << " 3   PRINT HISTORY" << endl;
    cout << " 4   EXIT" << endl;
    cout << "----------------------" << endl;
}
void menu_doctor()
{
    cout << " 1   NEW DOCTOR" << endl;
    cout << " 2   DELETE DOCTOR" << endl;
    cout << " 3   PRINT DOCTOR" << endl;
    cout << " 4   MAIN MENU" << endl;
    cout << "----------------------" << endl;
}
void menu_patient()
{
    cout << " 1   NEW PATIENT" << endl;
    cout << " 2   DELETE PATIENT" << endl;
    cout << " 3   PRINT PATIENT" << endl;
    cout << " 4   VISIT" << endl;
    cout << " 5   MAIN MENU" << endl;
    cout << "----------------------" << endl;
}

int main()
{
    _here:
    c_file file;
    menu();
    int ch;
    cin>>ch;
    switch (ch)
    {
        case 1:
        {
            menu_doctor();
            int ch;
            cin>>ch;
            switch (ch)
            {
                case 1:
                {
                    file.new_doctor();
                    break;
                }
                case 2:
                {
                    file.delete_doctor();
                    break;
                }
                case 3:
                {
                    file.print_doctor();
                    break;
                }
                case 4:
                {
                    goto _here;
                }
            }
            goto _here;
        }
        case 2:
        {
            menu_patient();
            int ch;
            cin>>ch;
            switch (ch)
            {
                case 1:
                {
                    file.new_patient();
                    break;
                }
                case 2:
                {
                    file.delete_patient();
                    break;
                }
                case 3:
                {
                    file.print_patient();
                    break;
                }
                case 4:
                {
                    file.visit();
                }
                case 5:
                {
                    goto _here;
                }
            }
            goto _here;
        }
        case 3:
        {
            file.print_history();
            goto _here;
        }
        case 4:
        {
            exit(0);
        }
        default:
        {
            goto _here;
        }
    }
}