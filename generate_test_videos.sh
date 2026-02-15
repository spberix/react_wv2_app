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

    # Generate video with FFmpeg (simple solid color)
    ffmpeg -f lavfi -i "color=c=0x${color}:s=1280x720:d=10" \
        -c:v libx264 -pix_fmt yuv420p -t 10 -y "$filename" > /dev/null 2>&1

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
