//
//  AppController.h
//  MacOSXDemo
//
//  Created by 潘李亮 on 10-5-5.
//  Copyright __MyCompanyName__ 2010 . All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

@interface AppController : NSObject 
{
    IBOutlet QCView* qcView;
}

- (IBAction) changeColorToBlue:(id)sender;

@end
