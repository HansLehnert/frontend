

class Exception {
public:
    const std::string& getMessage() { return message; }
protected:
    std::string message;
};


class ChildNotFound : public Exception {
public:
    ChildNotFound(const std::string& parent_name, const std::string& child_name) {
        message = "Object " + child_name + " is not a child of " + parent_name;
    }
};
