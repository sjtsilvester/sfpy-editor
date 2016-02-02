#include "ObjectWindow.h"
#include "Object.h"
#include "GameManager.h"
#include "Gui.h"

ObjectWindow::ObjectWindow(Gui* parent, GameManager* game_manager, std::string name,MenuItem* menu_item){
	construct(parent, game_manager, name,menu_item);
}

ObjectWindow::~ObjectWindow(){
}

void ObjectWindow::applySettings(){ //TODO test all properties of level editor currently and make sure everything is working, then finish it!
	//check if name is already in use
	std::string name = nameentry_->GetText();
	Object* compare = game_manager_->getObjectByName(name);
	if (compare != NULL && compare != object_){
		std::cout << "ERROR: Name already in use in game " + name << std::endl;
	}
	else{
		object_->setName(name);
		setName(name);
		window_->SetTitle("Object: " + name_);
		std::string visible = (visbutton_->IsActive() ? "True" : "False");
		std::string collidable = (colbutton_->IsActive() ? "True" : "False");
		std::string texture = spriteentry_->GetText();

		if (game_manager_->getTextureByName(texture) == NULL){
			std::cout << "ERROR: Attempted to set texture of object " + name + " to texture " + texture + " which doesn't exist." << std::endl;
		}
		else{
			object_->setProperty("texture", texture);
			//Update all sprites of instances of objects that have already been placed
			game_manager_->updateAllEntitySprites(object_);
		}

		object_->setProperty("visible", visible);
		object_->setProperty("collidable", collidable);
		object_->setProperty("name", name);
	}
}

void ObjectWindow::createElements(){
	using namespace sfg;
	type_ = Gui::GUI_OBJECT;
	object_ = game_manager_->getObjectByName(name_);
	window_->SetTitle("Object: " + name_);

	auto namebox = Box::Create(Box::Orientation::HORIZONTAL, 5);
	auto spritebox = Box::Create(Box::Orientation::HORIZONTAL, 5);
	auto boxbox = Box::Create(Box::Orientation::HORIZONTAL, 5);
	auto donebox = Box::Create(Box::Orientation::HORIZONTAL, 5);
	auto layoutbox = Box::Create(Box::Orientation::VERTICAL, 5);

	nameentry_ = Entry::Create();
	nameentry_->SetRequisition(maths::Vector2(200, 0));
	nameentry_->SetText(name_);
	auto namelabel = Label::Create("Name");

	namebox->Pack(namelabel);
	namebox->Pack(nameentry_);

	spriteentry_ = Entry::Create();
	spriteentry_->SetRequisition(maths::Vector2(200, 0));
	spriteentry_->SetText(object_->getProperty("texture"));
	auto spritelabel = Label::Create("Sprite");

	spritebox->Pack(spritelabel);
	spritebox->Pack(spriteentry_);

	visbutton_ = CheckButton::Create("Visible");
	if (object_->getProperty("visible") == "True"){
		visbutton_->SetActive(true);
	}
	colbutton_ = CheckButton::Create("Collidable");
	if (object_->getProperty("collidable") == "True"){
		colbutton_->SetActive(true);
	}

	boxbox->Pack(visbutton_);
	boxbox->Pack(colbutton_);

	apply_button_ = Button::Create("Apply");
	donebox->Pack(apply_button_);

	layoutbox->Pack(namebox);
	layoutbox->Pack(spritebox);
	layoutbox->Pack(boxbox);
	layoutbox->Pack(donebox);

	window_->Add(layoutbox);
}