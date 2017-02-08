//
//  glwindowtestAppDelegate.m
//  glwindowtest
//
//  Created by Kestutis Lotuzis on 6/23/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "glwindowtestAppDelegate.h"
#import "EAGLView.h"

@implementation glwindowtestAppDelegate

@synthesize window;
@synthesize glView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [glView startAnimation];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
}



- (void)dealloc
{
    [window release];
    [glView release];

    [super dealloc];
}

@end
