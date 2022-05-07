#pragma once

#include <iostream>
#include <map>
#include <ostream>
#include <string>
// #include <windows.hh>

#include "entities/combo_command.hh"
#include "entities/element.hh"
#include "entities/folder.hh"
#include "error-handling.hh"
#include "executor.hh"
#include "singleton.hh"

#define COL_RESET "\033[0m"
#define COL_BOLD "\033[1m"
#define COL_WHITE "\033[37m"
#define COL_BOLDBLUE "\033[1m\033[34m" /* Bold Blue */
#define COL_BOLDRED "\033[1m\033[31m" /* Bold Red */
#define COL_UNDERBOLDRED "\033[4m\033[1m\033[31m" /* Underligned Bold Red */
#define COL_RED "\033[31m" /* RED */
#define COL_BOLDGREEN "\033[1m\033[32m" /* Bold Green */
#define COL_GREEN "\033[0m\033[32m" /*Green */
#define COL_YELLOW "\033[33m" /* Yellow */
#define COL_BOLDWHITE "\033[1m\033[37m" /* Bold White */
#define COL_BOLDCYAN "\033[1m\033[36m" /* Bold Cyan */
#define COL_BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */

#define CLEAR_SCREEN "\033[2J\033[1;1H" /* Command equivalent to ^L */

namespace Cpad
{
    /**
     * @brief Color enumeration in addition to macros for operator overloading
     *
     */
    enum Color
    {
        RESET,
        BOLD,
        WHITE,
        BOLDBLUE /* Bold Blue */,
        BOLDRED /* Bold Red */,
        UNDERBOLDRED /* Underligned Bold Red */,
        RED /* RED */,
        BOLDGREEN /* Bold Green */,
        GREEN /*Green */,
        YELLOW /* Yellow */,
        BOLDWHITE /* Bold White */,
        BOLDCYAN /* Bold Cyan */,
        BOLDMAGENTA /* Bold Magenta */,
    };

    class Output : public Singleton<Output>
    {
    private:
        /// Internal save of emoji state.
        bool emoji_ = false;

        bool is_template(const std::string &command);
        void remplace_templates(std::string &command, bool clear);
        bool cd_exec(const std::string &command);
        void execute(std::string &command, bool emoji, bool clear);
        void execute_combo(const std::string &command,
                           std::string &display_line, Element &combo_elem,
                           bool emoji);

    public:
        /**
         * @brief Display the content of a folder.
         *
         * @param folder
         */
        void display_folder(Folder *folder) const;

        /**
         * @brief Display the given error.
         */
        void display_error(ErrorHandling::Error e) const;

        /**
         * @brief Launch a command and display the output to std::cout.
         *
         * @param cmd: The command to run.
         */
        void run_and_display(Command *cmd);

        /**
         * @brief Launch a combo command and display the output to std::cout.
         *
         * @param combo: The combo command to run.
         */
        void run_and_display(ComboCommand *combo);

        /**
         * @brief Display the help message.
         */
        void display_help() const;

        /**
         * @brief Clear the terminal.
         */
        void clear() const;

        /**
         * @brief Set the internal emoji status to avoid passing it as a
         * function argument.
         */
        void set_emoji_status(bool val);
    };

    /**
     * @brief Reset color, set the good one, print content and reset again.
     */
    std::ostream &operator<<(std::ostream &os, Color c);

} // namespace Cpad

/*












*/

class Display : public Singleton<Display>
{
public:
    using error_map =
        std::map<ErrorHandling::Error, std::pair<std::string, std::string>>;
    using executor_map =
        std::map<Executor::ExecutionType, std::pair<std::string, std::string>>;
    using Error = ErrorHandling::Error;
    using Exec = Executor::ExecutionType;

    void display_executor(Executor::executor_result executor,
                          Element &element_combo, bool emoji,
                          bool clear = true);
    void display_helper();
    void set_display_line(std::string &display_line, const std::string &end_str,
                          const std::string &emoji_str,
                          const std::string &replace, const std::string &color,
                          bool emoji);

private:
    bool is_template(const std::string &command);
    void remplace_templates(std::string &command, bool clear);
    bool cd_exec(const std::string &command);
    void execute(std::string &command, bool emoji, bool clear);
    void execute_combo(const std::string &command, std::string &display_line,
                       Element &combo_elem, bool emoji);
};