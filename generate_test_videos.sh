#!/bin/bash

# Script to generate 9 test videos for the video grid application

VIDEO_DIR="/tmp/video_grid_test"

# Check if FFmpeg is installed
if ! command -v ffmpeg &> /dev/null; then
    echo "Error: FFmpeg is not installed"
    echo "Install with: brew install ffmpeg"
    exit 1
fi

# Create video directory
mkdir -p "$VIDEO_DIR"

echo "Generating 9 test videos in: $VIDEO_DIR"

# Define colors and names for 9 videos
declare -a colors=(
    "FF6464:Red"
    "64FF64:Green"
    "6464FF:Blue"
    "FFFF64:Yellow"
    "FF64FF:Magenta"
    "64FFFF:Cyan"
    "FFA500:Orange"
    "A064FF:Purple"
    "64FFB4:Mint"
)

# Generate each video
for i in {0..8}; do
    filename="$VIDEO_DIR/test_video_$i.mp4"

    # Skip if already exists
    if [ -f "$filename" ]; then
        echo "  Video $i already exists"
        continue
    fi

    # Parse color and name
    IFS=':' read -r color name <<< "${colors[$i]}"
    participant_num=$((i + 1))

    echo "  Generating video $i ($name)..."

    # Generate animated looping video with FFmpeg
    # Each video gets a unique animation style based on its index
    case $i in
        0) # Animated gradient (Red)
            ffmpeg -f lavfi -i "color=c=0x${color}:s=1280x720:d=10" \
                -f lavfi -i "nullsrc=s=1280x720:d=10" \
                -filter_complex "[1]geq=r='255*sin(X/50+T)':g='255*sin(Y/50+T*1.5)':b='255*cos((X+Y)/50+T*2)',format=rgb24[fg];[0][fg]blend=all_mode=overlay:all_opacity=0.6" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        1) # Plasma effect (Green)
            ffmpeg -f lavfi -i "nullsrc=s=1280x720:d=10,geq=r='128+127*sin((X+T*10)/20)*cos((Y+T*15)/20)':g='128+127*sin((X+T*15)/25)*sin((Y+T*10)/25)':b='128+127*cos((X+T*20)/30)*cos((Y+T*25)/30)'" \
                -vf "hue=h=120:s=1.5" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        2) # Radial waves (Blue)
            ffmpeg -f lavfi -i "nullsrc=s=1280x720:d=10,geq=r='100+100*sin(hypot(X-640,Y-360)/30-T*5)':g='100+100*sin(hypot(X-640,Y-360)/30-T*5+2)':b='200+55*sin(hypot(X-640,Y-360)/30-T*5+4)'" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        3) # Moving stripes (Yellow)
            ffmpeg -f lavfi -i "nullsrc=s=1280x720:d=10,geq=r='255*abs(sin((Y+T*30)/40))':g='255*abs(sin((Y+T*30)/40))':b='100*abs(sin((Y+T*30)/40))'" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        4) # Spiral pattern (Magenta)
            ffmpeg -f lavfi -i "nullsrc=s=1280x720:d=10,geq=r='255*abs(sin(atan2(Y-360,X-640)*5+hypot(X-640,Y-360)/20-T*3))':g='100':b='255*abs(cos(atan2(Y-360,X-640)*5+hypot(X-640,Y-360)/20-T*3))'" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        5) # Tunnel effect (Cyan)
            ffmpeg -f lavfi -i "nullsrc=s=1280x720:d=10,geq=r='100*abs(sin(hypot(X-640,Y-360)/50+T*2))':g='255*abs(sin(hypot(X-640,Y-360)/50+T*2))':b='255*abs(cos(hypot(X-640,Y-360)/50+T*2))'" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        6) # Checker wave (Orange)
            ffmpeg -f lavfi -i "nullsrc=s=1280x720:d=10,geq=r='255*abs(sin(X/40+T*2)*sin(Y/40+T*3))':g='165*abs(sin(X/40+T*2)*sin(Y/40+T*3))':b='0'" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        7) # Color cycling (Purple)
            ffmpeg -f lavfi -i "color=c=0x${color}:s=1280x720:d=10" \
                -vf "hue=h='t*360/10':s=2" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
        8) # Perlin noise (Mint)
            ffmpeg -f lavfi -i "nullsrc=s=1280x720:d=10,geq=r='100+155*sin(X/100+T)*cos(Y/100+T*1.3)':g='255*abs(sin(X/80+T*1.5)*cos(Y/80+T))':b='180+75*cos(X/90+T*0.8)*sin(Y/90+T*1.2)'" \
                -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1
            ;;
    esac

    if [ $? -eq 0 ]; then
        echo "    ✓ Done"
    else
        echo "    ✗ Failed"
        exit 1
    fi
done

echo ""
echo "All test videos generated successfully!"
echo "Videos location: $VIDEO_DIR"
