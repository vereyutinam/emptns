#pragma once
#include "HotelClasses.h"
#include "AddEditForm.h"

public ref class MainForm : public System::Windows::Forms::Form
{
public:
    MainForm(void);
    ~MainForm();

private:
    Hotel^ hotel;
    System::Windows::Forms::DataGridView^ dataGridViewRooms;
    System::Windows::Forms::Button^ buttonAdd;
    System::Windows::Forms::Button^ buttonEdit;
    System::Windows::Forms::Button^ buttonDelete;
    System::Windows::Forms::Button^ buttonSort;
    System::Windows::Forms::Button^ buttonSave;
    System::Windows::Forms::Button^ buttonLoad;
    System::Windows::Forms::Label^ labelStats;
    System::ComponentModel::Container^ components;

    void InitializeComponent(void);
    void buttonAdd_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonEdit_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonDelete_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonSort_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonSave_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonLoad_Click(System::Object^ sender, System::EventArgs^ e);
    void UpdateRoomsDisplay();
    void UpdateStatistics();
};
