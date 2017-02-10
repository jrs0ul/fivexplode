#import "EAGLView.h"
#import <CommonCrypto/CommonDigest.h>
#import "UIViewController.h"
#import <dlfcn.h> 
#import <mach-o/dyld.h> 
#import <TargetConditionals.h> 

/* The encryption info struct and constants are missing from the iPhoneSimulator SDK, but not from the iPhoneOS or
 * Mac OS X SDKs. Since one doesn't ever ship a Simulator binary, we'll just provide the definitions here. */
#if TARGET_IPHONE_SIMULATOR && !defined(LC_ENCRYPTION_INFO)
#define LC_ENCRYPTION_INFO 0x21
struct encryption_info_command {
    uint32_t cmd;
    uint32_t cmdsize;
    uint32_t cryptoff;
    uint32_t cryptsize;
    uint32_t cryptid;
};
#endif

int main (int argc, char *argv[]);
@implementation EAGLView
@synthesize gameCenterManager;
@synthesize animating;
@synthesize initialDistance;
@synthesize bannerIsVisible;
@dynamic animationFrameInterval;




//----------------------------------
- (void) showAlertWithTitle: (NSString*) title message: (NSString*) message
{
	UIAlertView* alert= [[[UIAlertView alloc] initWithTitle: title message: message 
                                                   delegate: NULL cancelButtonTitle: @"OK" otherButtonTitles: NULL] autorelease];
	[alert show];
	
}

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}
//-------------------------------------

bool HardwareSupport() {
    const mach_header *header;
    Dl_info dlinfo;
    
    /* Fetch the dlinfo for main() */
    if (dladdr((void*)main, &dlinfo) == 0 || dlinfo.dli_fbase == NULL) {
        //NSLog(@"Could not find main() symbol (very odd)");
        return true;
    }
    header = (mach_header*)dlinfo.dli_fbase;
    
    /* Compute the image size and search for a UUID */
    struct load_command *cmd = (struct load_command *) (header+1);
    
    for (uint32_t i = 0; cmd != NULL && i < header->ncmds; i++) {
        /* Encryption info segment */
        if (cmd->cmd == LC_ENCRYPTION_INFO) {
            struct encryption_info_command *crypt_cmd = (struct encryption_info_command *) cmd;
            /* Check if binary encryption is enabled */
            if (crypt_cmd->cryptid < 1) {
                /* Disabled, probably pirated */
                return true;
            }
            
            /* Probably not pirated? */
            return false;
        }
        
        cmd = (struct load_command *) ((uint8_t *) cmd + cmd->cmdsize);
    }
    
    /* Encryption info not found */
    return true;
}


//------------------------------------
//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{    
    if ((self = [super initWithCoder:coder]))
    {
        
        _textField = [[UITextField alloc] initWithFrame:CGRectMake(60, 120, 160, 30)];
        [_textField setDelegate:self];
        [_textField setBackgroundColor:[UIColor colorWithWhite:0.0 alpha:0.5]];
        _textField.keyboardType = UIKeyboardTypeASCIICapable;
        [_textField setTextColor:[UIColor whiteColor]];
        [_textField setFont:[UIFont fontWithName:@"Arial" size:19]];
        [_textField setPlaceholder:@"NONAME"];
        
        
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context])
        {
            [self release];
            return nil;
        }
        
      
        
        strncpy(cppstuff.DocumentPath, [[NSSearchPathForDirectoriesInDomains( NSDocumentDirectory
                                              , NSUserDomainMask
                                              , YES) objectAtIndex:0] cString], 255);
        //puts(cppstuff.DocumentPath);
        
        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        glGenFramebuffersOES(1, &defaultFramebuffer);
        glGenRenderbuffersOES(1, &colorRenderbuffer);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);   
        
        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;
        
        //---
        cppstuff.init();
        mus = [IOSMusic alloc];
        [mus open:@"data/music.m4a"];
        
        self.multipleTouchEnabled = YES;
        
        
        

        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
        
        
           }

    if([GameCenterManager isGameCenterAvailable])
    {
        self.gameCenterManager= [[[GameCenterManager alloc] init] autorelease];
        [self.gameCenterManager setDelegate: self];
        [self.gameCenterManager authenticateLocalUser];
        
        //[self updateCurrentScore];
    }
    
    
    //AD banner
    if (NSClassFromString(@"ADBannerView")){
        adVC = [[UIViewController alloc] init];
        adView = [[ADBannerView alloc] initWithFrame:CGRectZero];
        //printf("height %f\n", [[UIScreen mainScreen] applicationFrame].size.height);
        adView.frame = CGRectOffset(adView.frame, 0, [[UIScreen mainScreen] applicationFrame].size.height);
    
        adView.requiredContentSizeIdentifiers = [NSSet setWithObject:ADBannerContentSizeIdentifierPortrait];
        adView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
        [self addSubview:adVC.view]; //Doesn't show up without this
    
        [adVC.view addSubview:adView];
        //[self addSubview:adView];
        adView.delegate=self;
        self.bannerIsVisible=NO;
    }

    
    //cppstuff.isPirate = HardwareSupport();
    
    return self;
}


//-------------------
- (void)bannerViewDidLoadAd:(ADBannerView *)banner
{
    cppstuff.hasadd = true;
    if ((!self.bannerIsVisible)&&(cppstuff.gamestate == GAME))
    {
        [UIView beginAnimations:@"animateAdBannerOn" context:NULL];
        
        banner.frame = CGRectOffset(banner.frame, 0, -50);
        [UIView commitAnimations];
        self.bannerIsVisible = YES;
    }
}
//------------------
- (void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    cppstuff.hasadd = false;
    if (self.bannerIsVisible)
    {
        [UIView beginAnimations:@"animateAdBannerOff" context:NULL];
      
        banner.frame = CGRectOffset(banner.frame, 0, 50);
        [UIView commitAnimations];
        self.bannerIsVisible = NO;
    }
}
//--------------------------
- (void) scoreReported: (NSError*) error;
{
#ifndef LITE
	if(error == NULL)
	{
		
    
        [self.gameCenterManager reloadHighScoresForCategory: RegularLeaderboardID];
    

        
		/*[self showAlertWithTitle: @"High Score Reported!"
						 message: [NSString stringWithFormat: @"", [error localizedDescription]]];*/
	}
	else
	{
		/*[self showAlertWithTitle: @"Score Report Failed!"
						 message: [NSString stringWithFormat: @"Reason: %@", [error localizedDescription]]];*/
	}
#endif
}




//----------------------------------
#pragma mark GameCenter View Controllers
- (void) showLeaderboard;
{
    
#ifndef LITE
    tempVC = [[UIViewControllerM alloc] init];
    
    GKLeaderboardViewController *leaderboardController = [[[GKLeaderboardViewController alloc] init] autorelease];
   
    if (leaderboardController != nil)
    {
        leaderboardController.leaderboardDelegate = self;
       
        leaderboardController.category = RegularLeaderboardID;//self.currentLeaderBoard;
		leaderboardController.timeScope = GKLeaderboardTimeScopeAllTime;
       
       
        [self addSubview:tempVC.view]; //Doesn't show up without this
        
        [tempVC presentModalViewController:leaderboardController animated:YES];
       
    }
    
#endif	
}
//------------------------
#ifndef LITE
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
   
   
	[tempVC dismissModalViewControllerAnimated:YES];
    [tempVC.view.superview removeFromSuperview];
    [tempVC release];
    
}

//---------------------
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	[self.gameCenterManager authenticateLocalUser];
}
//--------------------------
- (void) processGameCenterAuth: (NSError*) error
{
	if(error == NULL)
	{
		[self.gameCenterManager reloadHighScoresForCategory: RegularLeaderboardID];
	}
	else
	{
		UIAlertView* alert= [[[UIAlertView alloc] initWithTitle: @"Game Center Account Required" 
                                                        message: [NSString stringWithFormat: @"Reason: %@", [error localizedDescription]]
                                                       delegate: NULL cancelButtonTitle: @"OK" otherButtonTitles: NULL] autorelease];
		[alert show];
	}
	
}
//-------------------
- (void) mappedPlayerIDToPlayer: (GKPlayer*) player error: (NSError*) error;
{
	/*if((error == NULL) && (player != NULL))
	{
		self.leaderboardHighScoreDescription= [NSString stringWithFormat: @"%@ got:", player.alias];
		
		if(self.cachedHighestScore != NULL)
		{
			self.leaderboardHighScoreString= self.cachedHighestScore;
		}
		else
		{
			self.leaderboardHighScoreString= @"-";
		}
        
	}
	else
	{
		self.leaderboardHighScoreDescription= @"GameCenter Scores Unavailable";
		self.leaderboardHighScoreDescription=  @"-";
	}
	[self.tableView reloadData];*/
}
//----------------------------
//important method
- (void) reloadScoresComplete: (GKLeaderboard*) leaderBoard error: (NSError*) error;
{
	/*if(error == NULL)
	{
		int64_t personalBest= leaderBoard.localPlayerScore.value;
		self.personalBestScoreDescription= @"Your Best:";
		self.personalBestScoreString= [NSString stringWithFormat: @"%ld", personalBest];
		if([leaderBoard.scores count] >0)
		{
			self.leaderboardHighScoreDescription=  @"-";
			self.leaderboardHighScoreString=  @"";
			GKScore* allTime= [leaderBoard.scores objectAtIndex: 0];
			self.cachedHighestScore= allTime.formattedValue;
			[gameCenterManager mapPlayerIDtoPlayer: allTime.playerID];
		}
	}
	else
	{
		self.personalBestScoreDescription= @"GameCenter Scores Unavailable";
		self.personalBestScoreString=  @"-";
		self.leaderboardHighScoreDescription= @"GameCenter Scores Unavailable";
		self.leaderboardHighScoreDescription=  @"-";
		[self showAlertWithTitle: @"Score Reload Failed!"
						 message: [NSString stringWithFormat: @"Reason: %@", [error localizedDescription]]];
	}
	[self.tableView reloadData];*/
}
#endif

//-------------------------------------------------
// Saves the user name after the user enters it in the provied text field. 
- (void)textFieldDidEndEditing:(UITextField*)textField {
	//Save name
    strcpy(cppstuff.nameToEnter, [[textField text] UTF8String]);
	//[[NSUserDefaults standardUserDefaults] setObject:[textField text] forKey:@"BYBYS"];
	
	[_textField endEditing:YES];
	[_textField removeFromSuperview];
    cppstuff.TextEntered = true;
}

// Terminates the editing session
- (BOOL)textFieldShouldReturn:(UITextField*)textField {
	//Terminate editing
	[textField resignFirstResponder];
	
	return YES;
}
//-------------------------------
NSString * md5( NSString *str ){
	const char *cStr = [str UTF8String];
    
	unsigned char result[CC_MD5_DIGEST_LENGTH];
    
	CC_MD5( cStr, strlen(cStr), result );
    
	return [NSString 
			stringWithFormat: @"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			result[0], result[1], result[2], result[3],
			result[4], result[5], result[6], result[7], result[8], result[9],
			result[10], result[11], result[12], result[13], result[14], result[15]
			];
    
}
//-------------------
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    //puts("Pooom");
    cppstuff.getHighScore = true;
    cppstuff.gotHighScore = false;
    cppstuff.failToGetScore = false;
    cppstuff.waitForOnlineScoreTics = 0;
    strcpy(cppstuff.highScoreFromServer,"");
    
}

//my score system
//--------------------
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error{
    //UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No connection?" message:@"Impossible to send your score" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    //[alert show];
    //[alert release];
    
}
//----------------
- (BOOL)bannerViewActionShouldBegin:(ADBannerView *)banner willLeaveApplication:(BOOL)willLeave
{
    //NSLog(@"Banner view is beginning an ad action");
    BOOL shouldExecuteAction = YES;
    if (!willLeave && shouldExecuteAction)
    {
        if (animating)
        {
            if (displayLinkSupported)
            {
                [displayLink invalidate];
                displayLink = nil;
            }
            else
            {
                [animationTimer invalidate];
                animationTimer = nil;
            }
            
            animating = FALSE;
            cppstuff.gamestate = TITLE;
            cppstuff.hidebanner = true;
            time(&cppstuff.suspendStart);
            
            [mus stop];
        }
    }
    return shouldExecuteAction;
}
//-----------------
- (void)bannerViewActionDidFinish:(ADBannerView *)banner
{
    if (!animating){
        
        if (displayLinkSupported){
            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];
        
        animating = TRUE;
        
        //cppstuff.music.playback();
        [mus play];
        cppstuff.hidebanner = true;
    }
    
}
//--------------------------------
-(void) sendScore{
    //NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSString *urlToAuthPage = [[NSString alloc] initWithCString:cppstuff.highscorePostParams];
    urlToAuthPage = [urlToAuthPage stringByAppendingString:md5(@"pikti_satanistai")];
    //NSLog(urlToAuthPage);
    
    NSData *postData = [urlToAuthPage dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:NO];
    NSString *postLength = [NSString stringWithFormat:@"%d",[urlToAuthPage length]];
    NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
    NSString* tmpurl = [NSString stringWithFormat:@"http://jrs0ul.com/score/FiveXplode.php?mode=add&type="];
    tmpurl = [tmpurl stringByAppendingString:[NSString stringWithFormat:@"%d",cppstuff.onlineScoreType]];
    [request setURL:[NSURL URLWithString:tmpurl]];
    [request setHTTPMethod:@"POST"];
    [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
    [request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
    [request setHTTPBody:postData];
    NSURLConnection *conn = [[NSURLConnection alloc]initWithRequest:request delegate:self];
    [conn release];
  if([GameCenterManager isGameCenterAvailable]){
        
            [self.gameCenterManager reportScore: cppstuff.GameCenterScore forCategory: RegularLeaderboardID];
       
    }
}
//--------------------------------
-(void) sendStats{
    NSString *urlToAuthPage = [[NSString alloc] initWithCString:cppstuff.statPostParams];
    urlToAuthPage = [urlToAuthPage stringByAppendingString:md5(@"pikti_satanistai")];
    
    NSData *postData = [urlToAuthPage dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:NO];
    NSString *postLength = [NSString stringWithFormat:@"%d",[urlToAuthPage length]];
    NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
    [request setURL:[NSURL URLWithString:@"http://jrs0ul.com/stats/BumpyRobotsStats.php"]];
    [request setHTTPMethod:@"POST"];
    [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
    [request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
    [request setHTTPBody:postData];
    NSURLConnection *conn = [[NSURLConnection alloc]initWithRequest:request delegate:self];
    [conn release];
}
//--------------------------------
-(void) getScore{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSString *myURL = [[NSString alloc] initWithFormat:@"http://jrs0ul.com/score/FiveXplode.php?mode=get&type=%d&page=%d",
                                        cppstuff.onlineScoreType, cppstuff.onlineScorePage];
    
    NSString *result = [[NSString alloc] initWithContentsOfURL:[NSURL URLWithString:myURL]];
    
    if ([result length] > 0){
        strncpy(cppstuff.highScoreFromServer, [result cStringUsingEncoding:NSASCIIStringEncoding], 2024);
        cppstuff.gotHighScore = true; 
    }
    //NSLog(@"Response from server: %@",result);
    [myURL release];
    [result release];
    [pool release];
    [NSThread exit];
}

//--------------------------------------------------------------
- (void)drawView:(id)sender{
       
    
    [EAGLContext setCurrentContext:context];
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    cppstuff.render();
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
    //------------------------------
    cppstuff.logic();
    
    
    if (cppstuff.showBanner){
        if (NSClassFromString(@"ADBannerView")){
            if (adView.bannerLoaded == YES){
                if ((!self.bannerIsVisible)&&(cppstuff.gamestate == GAME))
                {
                    [UIView beginAnimations:@"animateAdBannerOn" context:NULL];
            
                    adView.frame = CGRectOffset(adView.frame, 0, -50);
                    [UIView commitAnimations];
                    self.bannerIsVisible = YES;
                }
            }
        }
        cppstuff.showBanner = false;
    }
    
    
    if (cppstuff.hidebanner)
    {
        if (NSClassFromString(@"ADBannerView")){
            if ((self.bannerIsVisible)&&(adView.bannerLoaded == YES)){
                [UIView beginAnimations:@"animateAdBannerOff" context:NULL];
                adView.frame = CGRectOffset(adView.frame, 0, 50);
                [UIView commitAnimations];
                self.bannerIsVisible = NO;
            
            }
        }
        cppstuff.hidebanner = false;
    }
    
    if (cppstuff.ChangeVolume){
        [mus setVolume:cppstuff.MusicVolume];
        cppstuff.ChangeVolume = false;
    }
    
    if (cppstuff.PlayNewSong){
        cppstuff.PlayNewSong = false;
        [mus dealloc];
        mus = [IOSMusic alloc];
        [mus open:[NSString stringWithCString:cppstuff.songName encoding:NSASCIIStringEncoding]];
        [mus setVolume:cppstuff.MusicVolume];
        [mus play];
    }
    
    if (cppstuff.buyGame){
        NSString *appstoreLink = @"itms-apps://itunes.com/apps/BumpyDroids";
        
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:appstoreLink]];
        cppstuff.buyGame = false;
    }
    
    if (cppstuff.PlayMusic){
        [mus play];
        cppstuff.PlayMusic = false;
    }
    
    if (cppstuff.launchjrs0ul){
        
        NSString *appstoreLink = @"itms-apps://itunes.com/apps/jrs0ul";
        
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:appstoreLink]];
        //[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://jrs0ul.com/en/projects/"]];
        cppstuff.launchjrs0ul = false;
    }
    
    if (cppstuff.ActivateEditField){
        [_textField setText:[NSString stringWithCString:cppstuff.nameToEnter encoding:NSASCIIStringEncoding]];
        [self addSubview:_textField];
        [_textField becomeFirstResponder];
        cppstuff.ActivateEditField = false;
    }
    
    
    if (cppstuff.sendScore){
        /*[NSThread detachNewThreadSelector:@selector(sendScore) 
                                 toTarget:self 
                               withObject:nil];*/
        
        [self sendScore];
        cppstuff.sendScore = false;
    }
    
    if (cppstuff.sendStats){
        [self sendStats];
        cppstuff.sendStats = false;
    }
    
#ifndef LITE
    if (cppstuff.ShowGameCenter){
        cppstuff.ShowGameCenter = false;
        if ([GameCenterManager isGameCenterAvailable])
            [self showLeaderboard];
        
    }
#endif
    
    if (cppstuff.getHighScore){
        [NSThread detachNewThreadSelector:@selector(getScore) 
                                 toTarget:self 
                               withObject:nil];
        cppstuff.getHighScore = false;
        //[this getScore];
    }
    

}
//--------------------------------------------------
- (void)layoutSubviews{
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer *)self.layer];
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    
    [self drawView:nil];
}
//----------------------------------------
- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}
//----------------------------------------
- (void)setAnimationFrameInterval:(NSInteger)frameInterval{
      if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;

        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}
//------------------------------------
- (void)startAnimation{
    if (!animating){
        
        if (displayLinkSupported){
            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        animating = TRUE;
        
        //cppstuff.music.playback();
        [mus play];
        
    }
}
//----------------------------------------
- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
        }

        animating = FALSE;
        cppstuff.gamestate = TITLE;
        cppstuff.hidebanner = true;
        time(&cppstuff.suspendStart);
        
        [mus stop];
    }
}
//---------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
       
    cppstuff.touches.down.destroy();
    if ([touches count])
        cppstuff.touches.allfingersup = false;
    for (unsigned i = 0; i < [touches count]; i++){
        UITouch *t = [[touches allObjects] objectAtIndex:i];
        Vector3D v([t locationInView:self].x, 
                   [t locationInView:self].y, 0);
        cppstuff.touches.down.add(v);
    }
   }
//------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    cppstuff.touches.move.destroy();
    if ([touches count])
        cppstuff.touches.allfingersup = false;
    for (unsigned i = 0; i < [touches count]; i++){
        UITouch *t = [[touches allObjects] objectAtIndex:i];
        Vector3D v([t locationInView:self].x, 
                   [t locationInView:self].y, 0);
        cppstuff.touches.move.add(v);
    }
}
//------------------------------------

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    cppstuff.touches.up.destroy();
    for (unsigned i = 0; i < [touches count]; i++){
        UITouch *t = [[touches allObjects] objectAtIndex:i];
        Vector3D v([t locationInView:self].x, 
                   [t locationInView:self].y, 0);
        cppstuff.touches.up.add(v);
    }
    //----
    if ([touches count] == [[event touchesForView:self] count]) //all fingers up
        cppstuff.touches.allfingersup = true;
   
    
}
//------------------------------------

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    cppstuff.touches.up.destroy();
    cppstuff.touches.move.destroy();
    cppstuff.touches.down.destroy();
   
    cppstuff.touches.allfingersup = true;
  
}
//------------------------------------
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration{
    
    //float kFilteringFactor = 0.8f;
    
   /* cppstuff.accelerometer.v[0] = acceleration.x * kFilteringFactor + cppstuff.accelerometer.v[0] * (1.0 - kFilteringFactor);
    cppstuff.accelerometer.v[1] = acceleration.y * kFilteringFactor + cppstuff.accelerometer.v[1] * (1.0 - kFilteringFactor);
    cppstuff.accelerometer.v[2] = acceleration.z * kFilteringFactor + cppstuff.accelerometer.v[2] * (1.0 - kFilteringFactor);*/
    
    
}

//---------------------------------------
- (void)dealloc{
    puts("Destroy");
    
    [_textField release];
    // Tear down GL
    if (defaultFramebuffer){
        glDeleteFramebuffersOES(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    
    if (colorRenderbuffer){
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    
    cppstuff.destroy();
    [mus dealloc];
    
    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    if (NSClassFromString(@"ADBannerView")){
        adView.delegate=nil;
        [adView release];
        [adVC release];
    }
    
    [context release];
    context = nil;
    [super dealloc];
}

@end
