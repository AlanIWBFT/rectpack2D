#include <vector>

struct Rect
{
	int Left;
	int Top;
	int Right;
	int Bottom;
};

struct PArea
{
	int Width;
	int Height;
};

class UVLayout
{
private:
	std::vector<PRect> Spaces;

public:
	UVLayout() = default;
	UVLayout(int PackingSizeX, int PackingSizeY);
	void Reset(int PackingSizeX, int PackingSizeY);
	bool Insert(const PArea& Area, Rect& OutInsertedRect);
};

UVLayout::UVLayout(int PackingSizeX, int PackingSizeY)
{
	Reset(PackingSizeX, PackingSizeY);
}

void UVLayout::Reset(int PackingSizeX, int PackingSizeY)
{
	Spaces.clear();
	Spaces.push_back(Rect { 0, 0, PackingSizeX, PackingSizeY });
}

bool UVLayout::Insert(const PArea& Area, Rect& OutInsertedRect)
{
	for (size_t i = Spaces.size(); i-- > 0;)
	{
		const Rect Space = Spaces[i];

		const int FreeWidth = Space.Right - Space.Left - Area.Width;
		const int FreeHeight = Space.Bottom - Space.Top - Area.Height;

		if (FreeWidth < 0 || FreeHeight < 0)
		{
			continue;
		}
		else if (FreeWidth == 0 && FreeHeight == 0)
		{
			OutInsertedRect.Left = Space.Left;
			OutInsertedRect.Top = Space.Top;
			OutInsertedRect.Right = OutInsertedRect.Left + Area.Width;
			OutInsertedRect.Bottom = OutInsertedRect.Top + Area.Height;

			Spaces[i] = Spaces[Spaces.size() - 1];
			Spaces.pop_back();
			return true;
		}
		else if (FreeWidth > 0 && FreeHeight == 0)
		{
			OutInsertedRect.Left = Space.Left;
			OutInsertedRect.Top = Space.Top;
			OutInsertedRect.Right = OutInsertedRect.Left + Area.Width;
			OutInsertedRect.Bottom = OutInsertedRect.Top + Area.Height;

			Rect NewSpace = Space;
			NewSpace.Left += Area.Width;

			Spaces[i] = Spaces[Spaces.size() - 1];
			Spaces[Spaces.size() - 1] = NewSpace;
			return true;
		}
		else if (FreeWidth == 0 && FreeHeight > 0)
		{
			OutInsertedRect.Left = Space.Left;
			OutInsertedRect.Top = Space.Top;
			OutInsertedRect.Right = OutInsertedRect.Left + Area.Width;
			OutInsertedRect.Bottom = OutInsertedRect.Top + Area.Height;

			Rect NewSpace = Space;
			NewSpace.Top += Area.Height;

			Spaces[i] = Spaces[Spaces.size() - 1];
			Spaces[Spaces.size() - 1] = NewSpace;
			return true;
		}
		else if (FreeWidth > FreeHeight)
		{
			OutInsertedRect.Left = Space.Left;
			OutInsertedRect.Top = Space.Top;
			OutInsertedRect.Right = OutInsertedRect.Left + Area.Width;
			OutInsertedRect.Bottom = OutInsertedRect.Top + Area.Height;

			{
				Rect NewSpace = Space;
				NewSpace.Left += Area.Width;

				Spaces[i] = Spaces[Spaces.size() - 1];
				Spaces[Spaces.size() - 1] = NewSpace;
			}

			{
				Rect NewSpace = Space;
				NewSpace.Top += Area.Height;
				NewSpace.Right = NewSpace.Left + Area.Width;

				Spaces.push_back(NewSpace);
			}

			return true;
		}
		else
		{
			OutInsertedRect.Left = Space.Left;
			OutInsertedRect.Top = Space.Top;
			OutInsertedRect.Right = OutInsertedRect.Left + Area.Width;
			OutInsertedRect.Bottom = OutInsertedRect.Top + Area.Height;

			{
				Rect NewSpace = Space;
				NewSpace.Top += Area.Height;

				Spaces[i] = Spaces[Spaces.size() - 1];
				Spaces[Spaces.size() - 1] = NewSpace;
			}

			{
				Rect NewSpace = Space;
				NewSpace.Left += Area.Width;
				NewSpace.Bottom = NewSpace.Top + Area.Height;

				Spaces.push_back(NewSpace);
			}

			return true;
		}
	}

	return false;
}
