#pragma once

#include <DirectXMath.h>
#include "core/debug/debug_window.h"
#include "util/string.h"
#include "util/utility.h"
#include "util/tuple.h"
#include "util/ring_buffer.h"
#include "util/functional.h"
#include <time.h>

namespace blowbox
{
    /**
    * This class operates your typical console window. It shows messages for you.
    *
    * @brief Operates a console to which you can send messages.
    */
    class Console : public DebugWindow
    {
    public:
        /** @brief Describes a message. */
        struct Message
        {
            String time_stamp;          //!< The timestamp of this message.
            String message;             //!< The contents of the message.
            DirectX::XMFLOAT4 color;    //!< The color that the message should be displayed as.
        };

        Console();
        ~Console();

        /** @brief Starts a new frame in the Console. */
        void NewFrame() override;

        /** @brief Renders the menu for the Console. */
        void RenderMenu() override;

        /** @brief Renders the actual window for the Console. */
        void RenderWindow() override;

        /**
        * @brief Logs a status message in the console.
        * @param[in] message The message to log.
        */
        void LogStatus(const String& message);

        /**
        * @brief Logs a warning message in the console.
        * @param[in] message The message to log.
        */
        void LogWarning(const String& message);

        /**
        * @brief Logs an error message in the console.
        * @param[in] message The message to log.
        */
        void LogError(const String& message);

        /**
        * @brief Logs a message in the console.
        * @param[in] message The message to log.
        * @param[in] color The color that the message should have.
        */
        void Log(const String& message, const DirectX::XMFLOAT4& color);

        /** @brief Clears all messages from the console. */
        void Clear();

    protected:
        /**
        * @brief Adds a Message to the console.
        * @param[in] message The message to add.
        */
        void AddMessage(Message& message);

    private:
        bool new_message_added_;                //!< Whether a new message has been added.
        bool auto_scroll_;                      //!< Whether the console window should auto scroll to the newest message.
        bool show_console_window_;              //!< Whether the console window is shown.
        RingBuffer<Message> message_buffer_;    //!< All messages in the console.
    };
}