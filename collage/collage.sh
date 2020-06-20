#!/bin/bash

image_type=${2:-".jpg"}

unsorted=$(find $1 -name "*${image_type}")

sorted=$(ls -tr $unsorted_list)

image_name="$(echo $1 | tr / _)${image_type}"

convert -coalesce +append $unsorted $image_name
