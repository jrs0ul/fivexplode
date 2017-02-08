

#import <UIKit/UIKit.h>
#include <limits.h>
#include <unistd.h>
#include <ctime>
#include <CoreFoundation/CoreFoundation.h>

int main(int argc, char *argv[]) {
    
    srand(time(0));
  
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        puts("Um...Error?");
    }
    CFRelease(resourcesURL);
    chdir(path);    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
   
    
    [pool release];
    
    return retVal;
}
