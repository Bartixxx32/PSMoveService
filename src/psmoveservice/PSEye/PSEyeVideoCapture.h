#include "opencv2/opencv.hpp"

enum
{
    PSEYE_CAP_ANY = 0,     // autodetect
#ifdef HAVE_CLEYE
    PSEYE_CAP_CLMULTI = 2100,
    PSEYE_CAP_CLEYE = 2200,
#endif
#ifdef HAVE_PS3EYE
    PSEYE_CAP_PS3EYE = 2300
#endif
};

/// Video capture class that prioritizes PS3 Eye devices via custom cv::IVideoCapture objects.
/**
To prioritize custom PS3 Eye devices, we must override open().
We use the parent cv::VideoCapture class's Ptr< cv::IVideoCapture >icap member variable.
It is set to one of the custom PS3 Eye cv::IVideoCapture objects or it uses the native OpenCV implementation.

Device opening priority:
-CL Eye MultiCam (Win x86 only; user must have activated camera OR CL Eye Platform SDK Developer binaries)
-CL Eye Driver (Win x86 only)
-PS3EYEDriver (Win and OSX)
-OpenCV native
 
 See base class:
 https://github.com/Itseez/opencv/blob/ddf82d0b154873510802ef75c53e628cd7b2cb13/modules/videoio/include/opencv2/videoio.hpp#L387
 https://github.com/Itseez/opencv/blob/ddf82d0b154873510802ef75c53e628cd7b2cb13/modules/videoio/src/cap.cpp#L550
*/
class PSEyeVideoCapture : public cv::VideoCapture {
public:

    /**
    \param camindex The index of the camera (0-based). To specify an API
    pass in the index + API. e.g., PSEyeVideoCapture(0 + PSEYE_CAP_CLMULTI)
    */
    PSEyeVideoCapture(int camindex) {open(camindex);} // same as base class
    
    /// Attempts to open a device using different drivers in the order outlined above.
    /*
    If a PS3 Eye device is not found, fall back to base class cv::VideoCapture::open(index)
    */
    bool open(int index);
    
private:
    /// Get the camera capture. If successful, we will have a functional cv::Ptr<CvCapture> \ref cap member variable.
    static cv::Ptr<cv::IVideoCapture> pseyeVideoCapture_create(int index);
#ifdef HAVE_CLEYE
    static cv::Ptr<cv::IVideoCapture> pseyeVideoCapture_create_CLMULTI(int index);
    static cv::Ptr<cv::IVideoCapture> pseyeVideoCapture_create_CLEYE(int index);
#endif
#ifdef HAVE_PS3EYE
    static cv::Ptr<cv::IVideoCapture> pseyeVideoCapture_create_PS3EYE(int index);
#endif
};