

#import <UIKit/UIKit.h>

#import <GameKit/GameKit.h>
#import "GameCenterManager.h"
#import <iAd/iAd.h>

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "CppSingleton.h"
#import "audio/iOSMusic.h"

#define RegularLeaderboardID @"score"


@class GameCenterManager;

    @interface EAGLView : UIView < UITextFieldDelegate, ADBannerViewDelegate,  GameCenterManagerDelegate, GKLeaderboardViewControllerDelegate>

{    
   
@private
    EAGLContext *context;
    GLint backingWidth;
    GLint backingHeight;
    UIViewController * adVC;
    
    ADBannerView *adView;
    BOOL bannerIsVisible;
    
    UIViewController * tempVC;
    
    GameCenterManager* gameCenterManager;
    
    IOSMusic* mus;
    
    GLuint defaultFramebuffer, colorRenderbuffer;
    
    BOOL animating;
    BOOL displayLinkSupported;
    NSInteger animationFrameInterval;
    id displayLink;
    NSTimer *animationTimer;
    
    UITextField* _textField;
    //IBOutlet UIWindow		*window;
    Singleton cppstuff;
    CGRect OldBounds;
    CGAffineTransform OldTransform;
    
  /*  NSString* personalBestScoreDescription;
	NSString* personalBestScoreString;
	
	NSString* leaderboardHighScoreDescription;
	NSString* leaderboardHighScoreString;
    
	NSString* currentLeaderBoard;
	
	int64_t  currentScore;
	NSString* cachedHighestScore;*/
       
}
@property (nonatomic, retain) GameCenterManager *gameCenterManager;
@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;
@property CGFloat initialDistance;
@property (nonatomic,assign) BOOL bannerIsVisible;


//gamecenter
/*@property (nonatomic, assign) int64_t currentScore;
@property (nonatomic, retain) NSString* cachedHighestScore;
@property (nonatomic, retain) NSString* personalBestScoreDescription;
@property (nonatomic, retain) NSString* personalBestScoreString;
@property (nonatomic, retain) NSString* leaderboardHighScoreDescription;
@property (nonatomic, retain) NSString* leaderboardHighScoreString;
@property (nonatomic, retain) NSString* currentLeaderBoard;*/



//multitouch
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
//accel
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration;


- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView:(id)sender;

-(void)sendScore;
-(void)sendStats;
-(void)getScore;

- (void) showLeaderboard;

@end
