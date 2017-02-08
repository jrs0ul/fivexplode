#import "iOSMusic.h"

@implementation IOSMusic

@synthesize audioPlayer;


-(bool)open:(NSString*)path{
    
	self.audioPlayer = [AVAudioPlayer alloc];
    if([audioPlayer initWithContentsOfURL:[NSURL fileURLWithPath:path] error:NULL]) {
		//[audioPlayer autorelease];
	}
	else {
		[audioPlayer release];
		audioPlayer = nil;
	}
    
    [self.audioPlayer setDelegate:self];
	self.audioPlayer.numberOfLoops = -1;
    
     
	if (audioPlayer == nil){
		return false;
    }
    return true;
}
//---------------------
-(void)play{
    [self.audioPlayer play];
}

//-----------------------
-(void)stop{
    [self.audioPlayer stop];
}
//----------------------
-(void)setVolume:(float)vol{
    self.audioPlayer.volume = vol;
}
//------------------------
- (void)dealloc {
	[self.audioPlayer stop];
	[self.audioPlayer release];
    self.audioPlayer = nil;
    [super dealloc];
}
//-------------------------
-(bool)isPlaying{
    return [self.audioPlayer isPlaying];
}
@end
