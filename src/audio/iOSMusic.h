#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@class IOSMusic;
@interface IOSMusic: NSObject<AVAudioPlayerDelegate>{
	AVAudioPlayer *audioPlayer;
}

@property (nonatomic,retain) AVAudioPlayer *audioPlayer ;

-(bool)open:(NSString*)path;
-(void)dealloc;
-(void)play;
-(bool)isPlaying;
-(void)stop;
-(void)setVolume:(float) vol;
@end


