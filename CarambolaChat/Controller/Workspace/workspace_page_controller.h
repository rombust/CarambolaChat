
#pragma once

class WorkspaceController;

class WorkspacePageController
{
public:
	WorkspacePageController();

	std::shared_ptr<clan::View> view = std::make_shared<clan::View>();

	const std::string &get_page_id() const { return page_id; }

	void set_tab_title(const std::string &text);
	void increment_message_count();

	void close();

protected:
	virtual void close_clicked() { }

private:
	static std::string generate_page_id() { int id = next_page_id++; return clan::StringHelp::int_to_text(id); }
	static int next_page_id;

	WorkspaceController *workspace = nullptr;
	std::string page_id = generate_page_id();

	int message_count = 0;

	friend class WorkspaceController;
};