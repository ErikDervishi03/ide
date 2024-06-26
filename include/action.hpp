#pragma once
#include "utils.h"
namespace action{

  namespace movement{
    void new_line(){
        buffer.new_row("", pointed_row + 1);
        
        //here we are breaking down the line.
        //All the text from the current cursor
        //position to the end of the line go in 
        //the next line
        
        const int curr_row_length = buffer.get_string_row(pointed_row).length();
        if (cursor.getX() != curr_row_length) {
          std::string line_break = buffer.slice_row(pointed_row, cursor.getX(), curr_row_length);
          buffer.row_append(pointed_row + 1, line_break);
        }
        
        if (cursor.getY() >= max_row - SCROLL_START_THRESHOLD - 1 && 
            !buffer.is_void_row(max_row) && 
            pointed_row < buffer.get_number_rows()) {

            starting_row++;
        }else if(cursor.getY() < max_row - 1){
          cursor.move_down();
        }

        cursor.setX(0);
        // update_screen(); --> remember to do this in IDE
        pointed_row++;
    }

    void move_up(){
      if (!(starting_row == 0 && pointed_row == 0)) {
        std::string prev_row = buffer.get_string_row(pointed_row - 1);
        if(cursor.getY() > SCROLL_START_THRESHOLD || starting_row == 0){
          cursor.move_up();
        }
        if (prev_row.length() < cursor.getX()) {
          cursor.setX(prev_row.length());
        } 
        if(starting_row > 0 && pointed_row <= starting_row + SCROLL_START_THRESHOLD){
          starting_row--;
          // update_screen();
        }
        pointed_row--;
      }
    }

    void move_down(){
      if (pointed_row < buffer.get_number_rows() - 1) {
        std::string next_row = buffer.get_string_row(pointed_row + 1);
        if(cursor.getY() < max_row - SCROLL_START_THRESHOLD - 1 || 
          pointed_row >= buffer.get_number_rows() - SCROLL_START_THRESHOLD - 1){
          
          cursor.move_down();
        }
        if(cursor.getX() >= next_row.length()){
          cursor.setX(next_row.length());
        }
        if(pointed_row - starting_row == max_row - SCROLL_START_THRESHOLD - 1 &&
          pointed_row < buffer.get_number_rows() - SCROLL_START_THRESHOLD - 1){
          
          starting_row++;
          // update_screen();
        }
        pointed_row++;
      } 
    }

    void move_left(){
      if(cursor.getX() == 0 && cursor.getY() > 0){
        cursor.set(buffer.get_string_row(pointed_row - 1).length(), cursor.getY() - 1);
        pointed_row--;
      }else if(cursor.getX() > 0){
        cursor.move_left();
      }
      if(starting_row > 0 && pointed_row == starting_row){
        starting_row--;
      }
    }

    void move_right(){
      if((cursor.getX() <= buffer.get_string_row(pointed_row).length() - 1) && !buffer.get_string_row(pointed_row).empty()){  
        cursor.move_right();
      }
    }
  };

  namespace modify{
    void insert_letter(int letter){
      if(cursor.getX()==max_col-7){
        buffer.new_row("", pointed_row + 1);
        if (cursor.getY() >= max_row - SCROLL_START_THRESHOLD - 1 && 
            !buffer.is_void_row(max_row) && 
            pointed_row < buffer.get_number_rows()) {

            starting_row++;
        }else if(cursor.getY() < max_row - 1){
          cursor.move_down();
        }

        cursor.setX(0);
        //update_screen(); --> remember to do this in IDE
        pointed_row++;
      }
      buffer.insert_letter(pointed_row, cursor.getX(), letter);
      cursor.move_right();
      // refresh_row(pointed_row); --> remember to do this in IDE
    }

    void command_insert_letter(int letter){
      command_buffer.push_back(letter);
      cursor.move_right();
    }

    void command_delete_letter(){
      if(command_buffer.length() > 0){
        command_buffer.pop_back();
        cursor.move_left();
      }
    }

    void delete_letter(){
      if (cursor.getX() == 0 && (cursor.getY() > 0 || starting_row > 0)) {
        if (starting_row > 0 && cursor.getY() == SCROLL_START_THRESHOLD) {
            starting_row--;
        }else if(cursor.getY() > 0){
          cursor.move_up();
        }
        cursor.setX(buffer.get_string_row(pointed_row - 1).length());
        buffer.merge_rows(pointed_row - 1, pointed_row);
        pointed_row--;
      } else if (cursor.getX() > 0) {
        cursor.move_left();
        buffer.delete_letter(pointed_row, cursor.getX());
      }
    }

    void normal_delete_letter(){
      if (cursor.getX() >= 0) {
        if(buffer.get_string_row(pointed_row).length() == cursor.getX() && cursor.getX() > 0)
          cursor.move_left();
        buffer.delete_letter(pointed_row, cursor.getX());
      }
    }

    void tab(){
      for(int i = 0; i < tab_size; i++){
        buffer.insert_letter(pointed_row, cursor.getX(), ' ');
        cursor.move_right();  
      }
      // refresh_row(pointed_row);
    }

    void delete_row(){
      buffer.del_row(pointed_row);
      cursor.setX(0);
      if(cursor.getY() > 0){
        cursor.move_up();
        pointed_row--;
      }
    }
  };

  namespace file{
    void upload() {
      if(pointed_file == ""){
        // throw an error
        return;
      }
      std::ofstream myfile(pointed_file);
      for (int i = 0; i < buffer.get_number_rows(); i++) { 
        myfile << buffer.get_buffer()[i] << "\n";
      }
      myfile.close();
    }

    void read(const char* file_name){
      if(std::filesystem::exists(file_name) && 
        std::filesystem::is_regular_file(file_name) &&
        std::filesystem::file_size(file_name) < 1000000 &&
        std::filesystem::file_size(file_name) > 0){

        std::ifstream myfile(file_name);
        if (!myfile.is_open()) {
          std::cout << "can't open : " << file_name << "\n";
          return;
        }
        buffer.clear();
        std::string line;
        while (std::getline(myfile, line)) {
          buffer.push_back(line);
        }

        myfile.close();
      }else{

        buffer.restore();

      }

    }
  };

  namespace system{
    void change2command(){
      mode = command;
      cursor.set(1, getmaxy(stdscr) - 2);
      cursor.restore(0);
    }
    void change2normal(){
      mode = normal;
    }
    void change2insert(){
      mode = insert;
    }
  };

};