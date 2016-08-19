# pragma once

namespace asc
{
	class IMessageButton
	{
	public:

		virtual ~IMessageButton() = default;

		virtual void onShownAll() = 0;

		virtual void onSubmit() = 0;

		virtual void update() = 0;

		virtual void draw() const = 0;

	};
}