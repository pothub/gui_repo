#pragma once
#include <time.h>     // for clock()
namespace gui_camera {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	/// <summary>
	/// ImageProcessingForm の概要
	/// </summary>
	public ref class ImageProcessingForm : public System::Windows::Forms::Form
	{
	public:
		ImageProcessingForm(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~ImageProcessingForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
	protected:
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(349, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"入力先指定";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ImageProcessingForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(88, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"処理画像フォルダ";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(150, 41);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(274, 19);
			this->textBox1->TabIndex = 2;
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(303, 66);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 3;
			// 
			// comboBox2
			// 
			this->comboBox2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(303, 92);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(121, 20);
			this->comboBox2->TabIndex = 4;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(349, 171);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 5;
			this->button2->Text = L"画像作成";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ImageProcessingForm::button2_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(150, 272);
			this->textBox2->Name = L"textBox2";
			this->textBox2->ReadOnly = true;
			this->textBox2->Size = System::Drawing::Size(274, 19);
			this->textBox2->TabIndex = 6;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(349, 243);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 7;
			this->button3->Text = L"入力先指定";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &ImageProcessingForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(268, 297);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 8;
			this->button4->Text = L"開始";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &ImageProcessingForm::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(349, 297);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 9;
			this->button5->Text = L"停止";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &ImageProcessingForm::button5_Click);
			// 
			// ImageProcessingForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(436, 391);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Name = L"ImageProcessingForm";
			this->Text = L"ImageProcessingForm";
			this->Load += gcnew System::EventHandler(this, &ImageProcessingForm::ImageProcessingForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ImageProcessingForm_Load(System::Object^  sender, System::EventArgs^  e) {
		comboBox1->Items->Add("3");
		comboBox1->Items->Add("5");
		comboBox1->Items->Add("7");
		comboBox1->Items->Add("9");
		comboBox1->Text = "5";

		comboBox2->Items->Add("Auto");
		for (int i = 0; i < 256; i++) comboBox2->Items->Add(i);
		comboBox2->Text = "Auto";
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		folderBrowserDialog1->ShowDialog();

		textBox1->Text = folderBrowserDialog1->SelectedPath;
		Cam SetPath(folderBrowserDialog1->SelectedPath);
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		Cam proc;
		clock_t start = clock();
		proc.SetCombo(comboBox1->Text, 1);
		proc.SetCombo(comboBox2->Text, 2);
		proc.FixAngle();
		proc.Average();
		proc.OutlineDifference();
		proc.InsideDifference();
		proc.MakeSigmaImage();
		proc.OutofSigma();
		clock_t end = clock();	//9.618
		cerr << (double)(end - start) / CLOCKS_PER_SEC;	//1.536
	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		folderBrowserDialog1->ShowDialog();
		textBox2->Text = folderBrowserDialog1->SelectedPath;
		Cam set;
		set.SetAuto(folderBrowserDialog1->SelectedPath);
	}
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
		Cam tim;
		tim.StartTimer();
	}	//start
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
		Cam tim;
		tim.StopTimer();
	}	//stop
	};
}
