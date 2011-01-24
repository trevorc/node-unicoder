#include <iostream>

#include <node.h>
#include <node_buffer.h>
#include <unicode/utypes.h>
#include <unicode/ucsdet.h>


namespace unicoder {

using namespace v8;

class CharsetDetector {

public:

    static void Initialize(Handle<Object> target) {
        HandleScope scope;
        NODE_SET_METHOD(target, "detect", Detect);
    }

private:

    UErrorCode _error;
    UCharsetDetector *_csd;

    CharsetDetector()
      : _error(U_ZERO_ERROR) {
        _csd = ucsdet_open(&_error);
    }

    ~CharsetDetector() {
        ucsdet_close(_csd);
    }

    Handle<Value> Detect(const char *buffer_data, size_t buffer_length) {
        const UCharsetMatch *match;
        Local<Object> result = Object::New();

        ucsdet_setText(_csd, buffer_data, buffer_length, &_error);
        match = ucsdet_detect(_csd, &_error);

        if (match == NULL) {
            return ThrowException(Exception::Error(
                String::New("Error detecting encoding.")));
        }

        const char *encoding = ucsdet_getName(match, &_error);
        const char *language = ucsdet_getLanguage(match, &_error);
        int32_t confidence = ucsdet_getConfidence(match, &_error);

        result->Set(String::NewSymbol("encoding"), String::New(encoding));
        result->Set(String::NewSymbol("language"), String::New(language));
        result->Set(String::NewSymbol("confidence"),
                    Integer::New(confidence));

        return result;
    }


    static Handle<Value> Detect(const Arguments& args) {
        HandleScope scope;

        if (args.Length() != 1) {
            return ThrowException(Exception::TypeError(
                String::New("Takes 1 parameter")));
        }

        if (!node::Buffer::HasInstance(args[0])) {
            return ThrowException(Exception::TypeError(
                String::New("First argument must be a buffer")));
        }

        CharsetDetector csd;
        Local<Object> buffer_obj = args[0]->ToObject();
        char *buffer_data = node::Buffer::Data(buffer_obj);
        size_t buffer_length = node::Buffer::Length(buffer_obj);

        return scope.Close(csd.Detect(buffer_data, buffer_length));
    }

};

} // namespace unicoder

extern "C" {

void init(v8::Handle<v8::Object> target) {
    unicoder::CharsetDetector::Initialize(target);
}

NODE_MODULE(unicoder_charset_detector, init)

}
