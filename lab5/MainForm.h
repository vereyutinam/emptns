#pragma once
#include "HotelClasses.h"
#include "AddEditForm.h"
#include "DatabaseManager.h"

public ref class MainForm : public System::Windows::Forms::Form
{
public:
    MainForm(void);
    ~MainForm();

private:
    Hotel^ hotel;
    DatabaseManager^ dbManager;

    System::Windows::Forms::DataGridView^ dataGridViewRooms;
    System::Windows::Forms::Button^ buttonAdd;
    System::Windows::Forms::Button^ buttonEdit;
    System::Windows::Forms::Button^ buttonDelete;
    System::Windows::Forms::Button^ buttonExport;
    System::Windows::Forms::Button^ buttonImport;
    System::Windows::Forms::SaveFileDialog^ saveFileDialog;
    System::Windows::Forms::OpenFileDialog^ openFileDialog;
    System::Windows::Forms::Label^ labelStats;
    System::ComponentModel::Container^ components;

    void InitializeComponent(void);
    void buttonAdd_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonEdit_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonDelete_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonExport_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonImport_Click(System::Object^ sender, System::EventArgs^ e);
    void UpdateRoomsDisplay();
    void UpdateStatistics();
    void LoadDataFromDatabase();
    void SaveDataToDatabase();
};
