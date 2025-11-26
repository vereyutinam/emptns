#pragma once
#include "HotelClasses.h"

public ref class AddEditForm : public System::Windows::Forms::Form
{
public:
    AddEditForm(void);
    AddEditForm(Room^ room, int index);

    property Room^ GetRoom {
        Room^ get() { return currentRoom; }
    }

    property int RoomIndex {
        int get() { return roomIndex; }
    }

    property bool IsEditing {
        bool get() { return isEditing; }
    }

protected:
    ~AddEditForm();

private:
    Room^ currentRoom;
    int roomIndex;
    bool isEditing;
    System::Windows::Forms::ComboBox^ comboBoxType;
    System::Windows::Forms::TextBox^ textBoxDiscount;
    System::Windows::Forms::Button^ buttonAction;
    System::Windows::Forms::Button^ buttonCancel;
    System::Windows::Forms::Label^ labelType;
    System::Windows::Forms::Label^ labelDiscount;
    System::ComponentModel::Container^ components;

    void InitializeComponent(void);
    void buttonAction_Click(System::Object^ sender, System::EventArgs^ e);
    void buttonCancel_Click(System::Object^ sender, System::EventArgs^ e);
    double CalculateCost(String^ type, int discount);
};
