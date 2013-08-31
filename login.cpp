#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WString>
#include <Wt/WTextArea>
#include<Wt/WApplication>
#include<Wt/WStackedWidget>
#include<Wt/WMenu>

using namespace Wt;

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/SqlConnection>
namespace dbo = Wt::Dbo;

#include <string>
#include <sstream>
using namespace std;

class ball
{
public:
string username;
string password;

template <class Check>
void persist(Check &a)
{
dbo::field(a, username, "username");
dbo::field(a, password, "password");
}
};

class MyApplication : public WApplication
{
public:
dbo::Session session;
dbo::backend::Sqlite3 sqlite3;

WLineEdit *deletetxt;
WLineEdit *Uptext;
WLineEdit *usertxt, *passtxt;
WContainerWidget *c1;
WLineEdit *retrievetxt;
WLineEdit *updatertxt;
WLineEdit *Uppass;

MyApplication(WEnvironment const & env);

void enterdata();
void retrievedata();
void updatedata();
void updatdata();
void deletedata();

};
MyApplication::MyApplication (WEnvironment const & env)
    : WApplication (env), sqlite3 ("datab5")
{

    session.setConnection (sqlite3);

    session.mapClass<ball>("manmeet");

   // session.createTables();

WText *txt=new WText("Username",root());
usertxt=new WLineEdit(root());

WText *pass_txt=new WText("Password",root());
passtxt=new WLineEdit(root());

WPushButton *push=new WPushButton("Register",root());
push->clicked().connect(this,&MyApplication::enterdata);



WText *text_r=new WText("Enter name to retrieve data",root());
retrievetxt=new WLineEdit(root());
WPushButton *push_r=new WPushButton("submit", root());
push_r->clicked().connect(this,&MyApplication::retrievedata);


WText *update_r=new WText("Enter name to update data",root());
updatertxt=new WLineEdit(root());
WPushButton *update=new WPushButton("Update",root());
update->clicked().connect(this,&MyApplication::updatedata);

WText *delete_r=new WText("Enter name to delete data",root());
deletetxt=new WLineEdit(root());
WPushButton *update_d=new WPushButton("Delete",root());
update_d->clicked().connect(this,&MyApplication::deletedata);
}

void MyApplication::deletedata()
{
Wt::WString a=deletetxt->text(); 
 std::string b=a.toUTF8();

{
dbo::Transaction transaction(session);

dbo::ptr<ball> manmeet= session.find<ball>().where("username = ?").bind("b");
manmeet.remove();
//=deletetxt->username;


transaction.commit();
}
}


void MyApplication::updatedata()
{
WContainerWidget *container=new WContainerWidget(root());
WText *uptext=new WText("Update Username", container);
Uptext=new WLineEdit(container);   //line editor for update username
root()->addWidget(new WBreak);

WText *uppass=new WText("Update Password", container);
Uppass=new WLineEdit(container);   ////line editor for update password

WPushButton *push_y=new WPushButton("Update",container);
push_y->clicked().connect(this,&MyApplication::updatdata);

}

void MyApplication::updatdata()

{
Wt::WString a=updatertxt->text(); 
 std::string b=a.toUTF8();

{
dbo::Transaction transaction(session);

dbo::ptr<ball> manmeet = session.find<ball>().where("username = ?").bind("b");

manmeet.modify()->username= 
Uptext->text().toUTF8();
manmeet.modify()->password = 
Uppass->text().toUTF8();


transaction.commit();
}
}

void MyApplication::enterdata()
{
{
dbo::Transaction transaction(session);
ball *manmeet=new ball();
manmeet->username=usertxt->text().toUTF8();
manmeet->password=passtxt->text().toUTF8();
dbo::ptr<ball> manmeetPtr = session.add(manmeet);

   transaction.commit();
}
}
void MyApplication::retrievedata()
{
 
 Wt::WString a=retrievetxt->text(); 
 std::string b=a.toUTF8();

{
dbo::Transaction transaction(session);
typedef dbo::collection< dbo::ptr<ball> > balls;
balls manmeet = session.find<ball>().where("username=?").bind(b);

int i;
for (balls::const_iterator i = manmeet.begin(); i != manmeet.end(); ++i)

{
root()->addWidget(new WText("name: "));
WText *nameout=new WText ((*i)->username,root());

root()->addWidget(new WBreak());
root()->addWidget(new WText("password: "));
WText *fathernameout=new WText ((*i)->password,root());
root()->addWidget(new WBreak());
}
}

}
WApplication * createApplication (WEnvironment const & env)
{
 return new MyApplication (env);
 }

int main(int argc, char **argv)
{
  return WRun (argc, argv, & createApplication);
}
