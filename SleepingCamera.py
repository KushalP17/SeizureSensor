import cv2 as cv
import sys
import numpy as np
import matplotlib.pyplot as plt

alive = True

# fgbg = cv.createBackgroundSubtractorKNN(detectShadows=True)
fgbg = cv.createBackgroundSubtractorMOG2()
kernel = np.ones((3,3),np.uint8)

running_frame_average = 0
steady_motion_frames = 0
total_frames = 1
prev_frame_average = 0

total_frames_list = [1]
frame_avg_list = [0]
running_frame_average_list = [0]

win_name = "Sleeping Seizure"
cv.namedWindow(win_name, cv.WINDOW_NORMAL)
# source = cv.VideoCapture('testvideos/seizure_man_2.mp4')


run_live_video = False
show_background_cutout = False

if run_live_video:
    source = cv.VideoCapture(1)
else:
    vid_src = "File"
    source = cv.VideoCapture('testvideos/seizure_woman.mp4')



while alive:
    has_frame, frame = source.read()
    frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    if not has_frame and vid_src == "File":
        source = cv.VideoCapture('testvideos/seizure_woman.mp4')
        
    # frame = cv.flip(frame, 1)

    # applying on each frame 
    fgmask = fgbg.apply(frame)

    # This is from the demo on erosion and dilation
    fgmask=(cv.morphologyEx(fgmask,cv.MORPH_CLOSE,kernel,iterations=10)>100)

    # use the mask to remove the background
    analyze_frame=frame[:,:]*fgmask
    # frame[:,:,1]=frame[:,:,1]*fgmask
    # frame[:,:,2]=frame[:,:,2]*fgmask

    if show_background_cutout:
        frame = analyze_frame    

    frame_average = np.average(analyze_frame)
    # print(frame_average)

    if frame_average > 1 or frame_average > running_frame_average:
        steady_motion_frames += 1
    else:
        steady_motion_frames = 0

    if steady_motion_frames > 40:
        pass # Function for Seizure Motion Detection
    else:
        pass # Function for No Motion Detected

    if len(frame_avg_list) > 1:
        frame_avg_list.remove(np.max(frame_avg_list))
        running_frame_average = np.average(frame_avg_list)


    running_frame_average = running_frame_average*(total_frames-1)/total_frames + frame_average/total_frames
    total_frames += 1
    prev_frame_average = frame_average

    running_frame_average_list.append(running_frame_average)
    total_frames_list.append(total_frames)

    cv.imshow(win_name, frame)

    # print(f"RFA: {running_frame_average}")
    # print(f"FA: {frame_average}")

    

    # print(running_frame_average)

    key = cv.waitKey(20)
    if key == ord("Q") or key == ord("q") or key == 27:
        alive = False

    if key == ord("0") or key == 48 or key == 96:
        source = cv.VideoCapture(1)
    elif key == ord("1") or key == 49 or key == 97:
        source = cv.VideoCapture('testvideos/seizure_woman.mp4')
    
    if key == ord("2") or key == 50 or key == 98:
        show_background_cutout = not show_background_cutout


