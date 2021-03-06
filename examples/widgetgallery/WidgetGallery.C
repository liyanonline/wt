
#include "EventsDemo.h"
#include "FormWidgets.h"
#include "GraphicsWidgets.h"
#include "Layout.h"
#include "Media.h"
#include "Navigation.h"
//#include "StyleLayout.h"
#include "TreesTables.h"
#include "WidgetGallery.h"

#include <Wt/WHBoxLayout.h>
#include <Wt/WMenu.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

WidgetGallery::WidgetGallery()
  : WContainerWidget()
{
  setOverflow(Wt::Overflow::Hidden);

  auto navigation = Wt::cpp14::make_unique<Wt::WNavigationBar>();
  navigation_ = navigation.get();

  navigation_->addStyleClass("main-nav");
  navigation_->setTitle("Wt Widget Gallery",
			"https://www.webtoolkit.eu/widgets");
  navigation_->setResponsive(true);

  auto contentsStack
      = Wt::cpp14::make_unique<Wt::WStackedWidget>();
  contentsStack_ = contentsStack.get();

  Wt::WAnimation animation(Wt::AnimationEffect::Fade,
			   Wt::TimingFunction::Linear,
			   200);
  contentsStack_->setTransitionAnimation(animation, true);

  /*
   * Setup the top-level menu
   */
  auto menu = Wt::cpp14::make_unique<Wt::WMenu>(contentsStack_);
  menu->setInternalPathEnabled();
  menu->setInternalBasePath("/");

  addToMenu(menu.get(), "Layout", Wt::cpp14::make_unique<Layout>());
  addToMenu(menu.get(), "Forms", Wt::cpp14::make_unique<FormWidgets>());
  addToMenu(menu.get(), "Navigation", Wt::cpp14::make_unique<Navigation>());
  addToMenu(menu.get(), "Trees & Tables", Wt::cpp14::make_unique<TreesTables>())
    ->setPathComponent("trees-tables");
  addToMenu(menu.get(), "Graphics & Charts", Wt::cpp14::make_unique<GraphicsWidgets>())
    ->setPathComponent("graphics-charts");
  //addToMenu(menu.get(), "Events", Wt::cpp14::make_unique<EventsDemo>());
  addToMenu(menu.get(), "Media", Wt::cpp14::make_unique<Media>());
  navigation_->addMenu(std::move(menu));

  /*
   * Add it all inside a layout
   */
  auto layout = this->setLayout(Wt::cpp14::make_unique<Wt::WVBoxLayout>());
  layout->setPreferredImplementation(Wt::LayoutImplementation::JavaScript);
  layout->addWidget(std::move(navigation), 0);
  layout->addWidget(std::move(contentsStack), 1);
  layout->setContentsMargins(0, 0, 0, 0);
}

Wt::WMenuItem *WidgetGallery::addToMenu(Wt::WMenu *menu,
					const Wt::WString& name,
					std::unique_ptr<TopicWidget> topic)
{
  auto topic_ = topic.get();
  auto result = Wt::cpp14::make_unique<Wt::WContainerWidget>();

  auto pane = Wt::cpp14::make_unique<Wt::WContainerWidget>();
  auto pane_ = pane.get();

  auto vLayout = result->setLayout(Wt::cpp14::make_unique<Wt::WVBoxLayout>());
  vLayout->setPreferredImplementation(Wt::LayoutImplementation::JavaScript);
  vLayout->setContentsMargins(0, 0, 0, 0);
  vLayout->addWidget(std::move(topic));
  vLayout->addWidget(std::move(pane), 1);

  auto hLayout = pane_->setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());
  hLayout->setPreferredImplementation(Wt::LayoutImplementation::JavaScript);

  auto item = Wt::cpp14::make_unique<Wt::WMenuItem>(name, std::move(result));
  auto item_ = menu->addItem(std::move(item));

  auto subStack = Wt::cpp14::make_unique<Wt::WStackedWidget>();
  subStack->addStyleClass("contents");
  subStack->setOverflow(Wt::Overflow::Auto);

  /*
  WAnimation animation(AnimationEffect::Fade,
                           TimingFunction::Linear,
			   100);
  subStack->setTransitionAnimation(animation, true);
  */

  auto subMenu = Wt::cpp14::make_unique<Wt::WMenu>(subStack.get());
  auto subMenu_ = subMenu.get();
  subMenu_->addStyleClass("nav-pills nav-stacked submenu");
  subMenu_->setWidth(200);

  hLayout->addWidget(std::move(subMenu));
  hLayout->addWidget(std::move(subStack),1);

  subMenu_->setInternalPathEnabled();
  subMenu_->setInternalBasePath("/" + item_->pathComponent());

  topic_->populateSubMenu(subMenu_);

  return item_;
}
