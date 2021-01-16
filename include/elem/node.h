#ifndef ELEM_NODE_H
#define ELEM_NODE_H

#include "elem/elemental_ui.h"

#include <vector>
#include <functional>

#include <elemd/vec2.hpp>
#include <elemd/context.hpp>
#include <elemd/font.hpp>
#include <elemd/event.hpp>

#include "measure_value.h"


namespace elem
{
    class document;

    template <typename T>
    struct maybe {
        bool is_set = false;
        T value;

        maybe<T>& operator=(const T& other) {
            is_set = true;
            value = other;
            return *this;
        }
    };

    class ELEM_API node
    {
    public:
        enum Display {
            BLOCK,
            INLINE
        };

        enum State {
            INITIAL,
            INITIAL_HOVER,
            HOVER,
            HOVER_INITIAL
        };
        
        // Style
        struct Style {
            Display display = Display::BLOCK;
            measure_value width;
            measure_value height;
            maybe<float> min_width;
            float min_height = -1;
            float max_width = std::numeric_limits<float>::max();
            float max_height = std::numeric_limits<float>::max();
            float padding[4] = { 0, 0, 0, 0 };
            float margin[4] = { 0, 0, 0, 0 };
            float border_width[4] = { 0, 0, 0, 0 };
            maybe<elemd::color> background_color;
            elemd::color color = elemd::color(0, 0, 0, 255);
            elemd::font* font_family = nullptr;
            float font_size = 10;
            float border_radius[4] = { 0, 0, 0, 0 };
            elemd::color border_color = elemd::color(0, 0, 0, 255);
            maybe<elemd::image*> background_image;
            float transition_time;
        };


        struct node_click_event {
            class node* node;   // GCC needs help here. class declaration is required
            elemd::mouse_button_event event;
        };
        struct node_scroll_event {
            class node* node;
            elemd::scroll_event event;
        };
        struct node_key_event {
            class node* node;
            elemd::key_event event;
        };
        struct node_char_event {
            class node* node;
            elemd::char_event event;
        };


        
        Style hover_style;
        Style style;

        std::string id;
        std::string class_name;

        void add_child(node* child);
        void remove_child(int index);
        void remove_child(node* child);

        virtual void set_state(State state);
        void set_focus(bool focus);
        void set_document(document* doc);
        void add_click_listener(std::function<bool(node_click_event)> callback);
        void add_scroll_listener(std::function<bool(node_scroll_event)> callback);

        int get_width();
        int get_height();
        elemd::vec2 get_position();

        static void add_to_hover_list(node* node);
        static void finish_hover_event();

        virtual void emit_click_event(elemd::mouse_button_event event);
        virtual void emit_scroll_event(elemd::scroll_event event);
        virtual void emit_key_event(elemd::key_event event);
        virtual void emit_char_event(elemd::char_event event);

        virtual node* bounds_check(elemd::vec2 pos);
        virtual elemd::vec2 get_minimum_dimensions(float width, float height);
        virtual float layout(elemd::vec2 position, float width, float height);
        virtual void offset(float x, float y);
        virtual void paint(elemd::Context* ctx) = 0;
        virtual void debug_paint(elemd::Context* ctx);
        virtual void destroy();
        void destroy_immediately();

    protected:
        static std::map<node*, int> _hover_map;
        document* _document;
        bool _should_destroy = false;

        node* _parent = nullptr;
        std::vector<node*> _children;
        std::vector<std::function<bool(node_click_event)>> _click_event_callbacks;
        std::vector<std::function<bool(node_scroll_event)>> _scroll_event_callbacks;
        std::vector<std::function<bool(node_key_event)>> _key_event_callbacks;
        std::vector<std::function<bool(node_char_event)>> _char_event_callbacks;
        bool _focused = false;

        State _state = INITIAL;
        float _transition_progress;
        elemd::vec2 _position = elemd::vec2(-1, -1);
        float _width = -1;
        float _height = -1;

        elemd::vec2 _scroll_offset;
        elemd::vec2 _scroll_percent;
        bool _scrollable_x = false;
        bool _scrollable_y = false;

        void set_parent(node* parent);


    };
} // namespace elem

#endif // ELEM_NODE_H