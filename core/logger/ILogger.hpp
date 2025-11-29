namespace logger {

template <typename T>
concept IMessage = requires(T message) {
    { message.what() } -> std::convertible_to<std::string>;
    { message.type() } -> std::convertible_to<std::string>;
};

template <typename T, typename TMessage>
concept ILogger = requires(T logger, const std::string& str, TMessage message) {
    requires IMessage<TMessage>;
    { logger.log(message) } -> std::same_as<void>;
    { logger.log(str) } -> std::same_as<void>;
};

}  // namespace logger